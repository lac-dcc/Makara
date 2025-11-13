import os
import sys
import re

str_header = """
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/perf_event.h>
#include <asm/unistd.h>
"""

str_perffunc = """
static long
perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
                int cpu, int group_fd, unsigned long flags)
{
    return syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
}
"""

str_setup = """
    struct perf_event_attr pe;
    int fd_cycles, fd_instructions, fd_cache_ref, fd_cache_miss;
    uint64_t count_cycles, count_instructions, count_cache_ref, count_cache_miss;

    // base event config
    memset(&pe, 0, sizeof(struct perf_event_attr));
    pe.size = sizeof(struct perf_event_attr);
    pe.disabled = 1;        // starts disabled
    pe.exclude_kernel = 0;  // measure kernel too (0 = measure all)
    pe.exclude_hv = 1;      // ignore hypervisor

    // ---------------------
    // main group: cpu cycles
    // ---------------------
    pe.type = PERF_TYPE_HARDWARE;
    pe.config = PERF_COUNT_HW_CPU_CYCLES;
    fd_cycles = perf_event_open(&pe, 0, -1, -1, 0);
    if (fd_cycles == -1) { perror("perf_event_open (cycles)"); exit(1); }

    // ---------------------
    // member 1: instructions
    // ---------------------
    pe.config = PERF_COUNT_HW_INSTRUCTIONS;
    fd_instructions = perf_event_open(&pe, 0, -1, fd_cycles, 0);
    if (fd_instructions == -1) { perror("perf_event_open (instructions)"); exit(1); }

    // ---------------------
    // member 2: cache references
    // ---------------------
    pe.config = PERF_COUNT_HW_CACHE_REFERENCES;
    fd_cache_ref = perf_event_open(&pe, 0, -1, fd_cycles, 0);
    if (fd_cache_ref == -1) { perror("perf_event_open (cache references)"); exit(1); }

    // ---------------------
    // member 3: cache misses
    // ---------------------
    pe.config = PERF_COUNT_HW_CACHE_MISSES;
    fd_cache_miss = perf_event_open(&pe, 0, -1, fd_cycles, 0);
    if (fd_cache_miss == -1) { perror("perf_event_open (cache misses)"); exit(1); }
    """

str_start = """
    // ---------------------
    // Enable the group
    // ---------------------
    ioctl(fd_cycles, PERF_EVENT_IOC_RESET, PERF_IOC_FLAG_GROUP);
    ioctl(fd_cycles, PERF_EVENT_IOC_ENABLE, PERF_IOC_FLAG_GROUP);

    // ======== Measured region ========
"""

str_stop = """
    // ======== End of measured region ========
    ioctl(fd_cycles, PERF_EVENT_IOC_DISABLE, PERF_IOC_FLAG_GROUP);
"""


def modifyFile(file):
    finalString = ""

    with open(file) as f:
        content = f.read()

    name_pattern = re.findall(
        r"\/\* Variables and functions \*\/\s*([\s\S]*?)\s*\/\/ ------------------------------------------------------------------------- \/\/",
        content,
    )
    if not name_pattern:
        print(f"Pattern 'Variables and functions' not found in {file} ❌.")
        return

    name_pattern2 = re.findall(r"(\_?\w+)\s*\(", name_pattern[0])

    name = ""
    excluded = ["if", "for", "__attribute__", "while"]

    for current_name in name_pattern2:
        if not current_name in excluded:
            name = current_name
            break

    if not name:
        print(f"No valid function name found in {file} ❌.")
        return

    str_results = f"""
    // ---------------------
    // Read results
    // ---------------------    
    read(fd_cycles, &count_cycles, sizeof(uint64_t));
    read(fd_instructions, &count_instructions, sizeof(uint64_t));
    read(fd_cache_ref, &count_cache_ref, sizeof(uint64_t));
    read(fd_cache_miss, &count_cache_miss, sizeof(uint64_t));

    // close file descriptors
    close(fd_cycles);
    close(fd_instructions);
    close(fd_cache_ref);
    close(fd_cache_miss);

    // ---------------------
    // Save to CSV
    // ---------------------
    FILE *f_csv = fopen("results/perf_results.csv", "a"); // "a" to append
    if (f_csv == NULL) {{
        perror("Error creating/opening CSV file");
        exit(1);
    }}
    
    // Check if file is empty to add header
    fseek(f_csv, 0, SEEK_END);
    long size = ftell(f_csv);
    if (size == 0) {{
        fprintf(f_csv, "function_name,cpu-cycles,instructions,cache-misses,cache-ref newline");
    }}
    
    // Write data
    fprintf(f_csv, "{name},%lu,%lu,%lu,%lu newline",
            count_cycles,
            count_instructions,
            count_cache_miss,
            count_cache_ref);
            
    fclose(f_csv);
"""

    lines = content.splitlines(True)

    for i, line in enumerate(lines):
        if "// includes\n" in line:
            finalString += line
            finalString += str_header
            finalString += str_perffunc
            continue

        if "int opt = atoi(argv[1]);\n" in line:
            finalString += line
            continue

        finalString += line

    def injection(match_obj):
        str1 = f"{{\n{str_setup}\n{str_start}"
        str2 = f"{str_stop}\n{str_results}\n}}"

        injection_status = {"found_function": False}

        switch_header = match_obj.group(1)
        switch_content = match_obj.group(2)

        # 1) Primeiro, tentamos detectar a forma de atribuição:
        #    <qualquer-declaração> <ident> = name(args);
        # Capturamos:
        #   g1 = indent
        #   g2 = declaração à esquerda (tipo + nome + '=') por exemplo "unsigned int benchRet = "
        #   g3 = chamada (name(args))
        #   g4 = ponto-e-vírgula e espaços finais
        assignment_pattern = (
            rf"(^|\n)(\s*)([\w\*\s]+\b\w+\s*=\s*)({re.escape(name)}\s*\([^;)]*\))(\s*;)"
        )

        def replace_assignment(m):
            leading_newline = m.group(1)
            indent = m.group(2) or ""
            left = m.group(3)  # ex: "unsigned int benchRet = "
            call_expr = m.group(4)  # ex: "rate_to_atmf(rate)"
            semicolon_ws = m.group(5)  # ex: "    ;" or just ";"

            # Usamos __typeof__(call_expr) para deduzir o tipo de retorno.
            # Construímos uma GCC statement expression: ({ setup; __typeof__(call_expr) __ret = call_expr; stop; results; __ret; })
            stmt_expr = (
                f"({{ \n"
                f"{str_setup}\n"
                f"{indent}// ---------------------\n"
                f"{indent}// Enable the group\n"
                f"{indent}// ---------------------\n"
                f"{indent}ioctl(fd_cycles, PERF_EVENT_IOC_RESET, PERF_IOC_FLAG_GROUP);\n"
                f"{indent}ioctl(fd_cycles, PERF_EVENT_IOC_ENABLE, PERF_IOC_FLAG_GROUP);\n\n"
                f"{indent}// ======== Measured region ========\n\n"
                f"{indent}__typeof__({call_expr}) __perf_ret = {call_expr};\n\n"
                f"{indent}// ======== End of measured region ========\n"
                f"{indent}{str_stop}\n"
                f"{indent}{str_results}\n\n"
                f"{indent}__perf_ret;\n"
                f"}})"
            )

            injection_status["found_function"] = True
            # Reconstroi a linha: preservamos o leading newline e a indentação
            return f"{leading_newline}{indent}{left}{stmt_expr}{semicolon_ws}"

        # Aplicar substituição de atribuição globalmente no switch_content
        (after_assignment, n_assign_subs) = re.subn(
            assignment_pattern, replace_assignment, switch_content, flags=re.DOTALL
        )

        # 2) Se não encontramos atribuição, tentamos a forma simples de chamada (sem atribuição)
        if n_assign_subs == 0:
            # padrão anterior: injetar antes e depois da chamada simples "name(...);"
            pattern_call = rf"(\s*)({re.escape(name)}\s*\([^)]*\)\s*;)"
            replacement_call = f"\\g<1>{str1}\\n\\g<1>\\g<2>\\n\\g<1>{str2}"
            (after_call, n_call_subs) = re.subn(
                pattern_call, replacement_call, switch_content
            )
            if n_call_subs > 0:
                injection_status["found_function"] = True
                modified_content = after_call
            else:
                modified_content = switch_content
        else:
            # já substituímos pela forma de atribuição
            modified_content = after_assignment

        return f"{switch_header}{modified_content}"

    new_finalString = re.sub(
        pattern=r"(switch\(opt\)\s*\{)([\s\S]*\})",
        repl=injection,
        string=finalString,
        count=1,
    )

    if len(new_finalString) != 0:
        try:
            if not os.path.isdir("ModifiedJotai"):
                os.mkdir("ModifiedJotai")

            with open(f"ModifiedJotai/{name}.c", "w") as f:
                f.write(new_finalString)
            print(f"Modified {file} created as {name}.c ✅... ")
        except Exception as e:
            print(f"Unable to modify the current {file} ❌. Error: {e}")
    else:
        print(f"{file} failed in the regex")


def injection_pipeline():
    if not os.path.isdir("Jotai"):
        print("Jotai directory not found.")
        return

    sources = os.listdir("Jotai")
    for source in sources:
        if source.endswith(".c"):
            modifyFile(file=f"Jotai/{source}")
        else:
            print(f"Ignoring non .c file: {source}")
