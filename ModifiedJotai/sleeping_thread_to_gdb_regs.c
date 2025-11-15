// ========================================================================= //

// includes

#define _GNU_SOURCE
#include <asm/unistd.h>
#include <linux/perf_event.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

static long perf_event_open(struct perf_event_attr *hw_event, pid_t pid,
                            int cpu, int group_fd, unsigned long flags) {
  return syscall(__NR_perf_event_open, hw_event, pid, cpu, group_fd, flags);
}
#include "float.h"
#include "limits.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"

#define JOTAI_NUM_RANDS_ 25

const unsigned rand_primes[JOTAI_NUM_RANDS_] = {
    179,   103,   479,    647,    229,    37,     271,   557,   263,
    607,   18743, 50359,  21929,  48757,  98179,  12907, 52937, 64579,
    49957, 52567, 507163, 149939, 412157, 680861, 757751};

int next_i() {
  int counter = 0;
  return rand_primes[(++counter) % JOTAI_NUM_RANDS_];
}

float next_f() {
  int counter = 0;
  return rand_primes[(++counter) % JOTAI_NUM_RANDS_] / 757751.0F;
}

// Usage menu
void usage() {
  printf("%s", "Usage:\n\
    prog [ARGS]\n\
\nARGS:\n\
       0            big-arr\n\
       1            big-arr-10x\n\
\n\
");
}

// ------------------------------------------------------------------------- //

#define NULL ((void *)0)
typedef unsigned long size_t; // Customize by platform.
typedef long intptr_t;
typedef unsigned long uintptr_t;
typedef long scalar_t__; // Either arithmetic or pointer type.
/* By default, we understand bool (as a convenience). */
typedef int bool;
#define false 0
#define true 1

/* Forward declarations */
typedef struct TYPE_2__ TYPE_1__;

/* Type definitions */
typedef int /*<<< orphan*/ u64;
struct TYPE_2__ {
  int /*<<< orphan*/ reg31;
  int /*<<< orphan*/ cp0_status;
  int /*<<< orphan*/ reg30;
  int /*<<< orphan*/ reg29;
  int /*<<< orphan*/ reg23;
  int /*<<< orphan*/ reg22;
  int /*<<< orphan*/ reg21;
  int /*<<< orphan*/ reg20;
  int /*<<< orphan*/ reg19;
  int /*<<< orphan*/ reg18;
  int /*<<< orphan*/ reg17;
  int /*<<< orphan*/ reg16;
};
struct task_struct {
  TYPE_1__ thread;
};

/* Variables and functions */

void sleeping_thread_to_gdb_regs(unsigned long *gdb_regs,
                                 struct task_struct *p) {
  int reg;
#if (KGDB_GDB_REG_SIZE == 32)
  u32 *ptr = (u32 *)gdb_regs;
#else
  u64 *ptr = (u64 *)gdb_regs;
#endif

  for (reg = 0; reg < 16; reg++)
    *(ptr++) = 0;

  /* S0 - S7 */
  *(ptr++) = p->thread.reg16;
  *(ptr++) = p->thread.reg17;
  *(ptr++) = p->thread.reg18;
  *(ptr++) = p->thread.reg19;
  *(ptr++) = p->thread.reg20;
  *(ptr++) = p->thread.reg21;
  *(ptr++) = p->thread.reg22;
  *(ptr++) = p->thread.reg23;

  for (reg = 24; reg < 28; reg++)
    *(ptr++) = 0;

  /* GP, SP, FP, RA */
  *(ptr++) = (long)p;
  *(ptr++) = p->thread.reg29;
  *(ptr++) = p->thread.reg30;
  *(ptr++) = p->thread.reg31;

  *(ptr++) = p->thread.cp0_status;

  /* lo, hi */
  *(ptr++) = 0;
  *(ptr++) = 0;

  /*
   * BadVAddr, Cause
   * Ideally these would come from the last exception frame up the stack
   * but that requires unwinding, otherwise we can't know much for sure.
   */
  *(ptr++) = 0;
  *(ptr++) = 0;

  /*
   * PC
   * use return address (RA), i.e. the moment after return from resume()
   */
  *(ptr++) = p->thread.reg31;
}

// ------------------------------------------------------------------------- //

int main(int argc, char *argv[]) {

  if (argc != 2) {
    usage();
    return 1;
  }

  int opt = atoi(argv[1]);
  switch (opt) {

  // big-arr
  case 0: {
    // static_instructions_O0 : 150
    // dynamic_instructions_O0 : 352
    // -------------------------------
    // static_instructions_O1 : 38
    // dynamic_instructions_O1 : 38
    // -------------------------------
    // static_instructions_O2 : 38
    // dynamic_instructions_O2 : 38
    // -------------------------------
    // static_instructions_O3 : 38
    // dynamic_instructions_O3 : 38
    // -------------------------------
    // static_instructions_Ofast : 38
    // dynamic_instructions_Ofast : 38
    // -------------------------------
    // static_instructions_Os : 38
    // dynamic_instructions_Os : 38
    // -------------------------------
    // static_instructions_Oz : 46
    // dynamic_instructions_Oz : 140
    // -------------------------------

    int _len_gdb_regs0 = 65025;
    unsigned long *gdb_regs =
        (unsigned long *)malloc(_len_gdb_regs0 * sizeof(unsigned long));
    for (int _i0 = 0; _i0 < _len_gdb_regs0; _i0++) {
      gdb_regs[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
    }

    int _len_p0 = 65025;
    struct task_struct *p =
        (struct task_struct *)malloc(_len_p0 * sizeof(struct task_struct));
    for (int _i0 = 0; _i0 < _len_p0; _i0++) {
      p[_i0].thread.reg31 = ((-2 * (next_i() % 2)) + 1) * next_i();
      p[_i0].thread.cp0_status = ((-2 * (next_i() % 2)) + 1) * next_i();
      p[_i0].thread.reg30 = ((-2 * (next_i() % 2)) + 1) * next_i();
      p[_i0].thread.reg29 = ((-2 * (next_i() % 2)) + 1) * next_i();
      p[_i0].thread.reg23 = ((-2 * (next_i() % 2)) + 1) * next_i();
      p[_i0].thread.reg22 = ((-2 * (next_i() % 2)) + 1) * next_i();
      p[_i0].thread.reg21 = ((-2 * (next_i() % 2)) + 1) * next_i();
      p[_i0].thread.reg20 = ((-2 * (next_i() % 2)) + 1) * next_i();
      p[_i0].thread.reg19 = ((-2 * (next_i() % 2)) + 1) * next_i();
      p[_i0].thread.reg18 = ((-2 * (next_i() % 2)) + 1) * next_i();
      p[_i0].thread.reg17 = ((-2 * (next_i() % 2)) + 1) * next_i();
      p[_i0].thread.reg16 = ((-2 * (next_i() % 2)) + 1) * next_i();
    }

    {

      struct perf_event_attr pe;
      int fd_cycles, fd_instructions, fd_cache_ref, fd_cache_miss;
      uint64_t count_cycles, count_instructions, count_cache_ref,
          count_cache_miss;

      // base event config
      memset(&pe, 0, sizeof(struct perf_event_attr));
      pe.size = sizeof(struct perf_event_attr);
      pe.disabled = 1;       // starts disabled
      pe.exclude_kernel = 0; // measure kernel too (0 = measure all)
      pe.exclude_hv = 1;     // ignore hypervisor

      // ---------------------
      // main group: cpu cycles
      // ---------------------
      pe.type = PERF_TYPE_HARDWARE;
      pe.config = PERF_COUNT_HW_CPU_CYCLES;
      fd_cycles = perf_event_open(&pe, 0, -1, -1, 0);
      if (fd_cycles == -1) {
        perror("perf_event_open (cycles)");
        exit(1);
      }

      // ---------------------
      // member 1: instructions
      // ---------------------
      pe.config = PERF_COUNT_HW_INSTRUCTIONS;
      fd_instructions = perf_event_open(&pe, 0, -1, fd_cycles, 0);
      if (fd_instructions == -1) {
        perror("perf_event_open (instructions)");
        exit(1);
      }

      // ---------------------
      // member 2: cache references
      // ---------------------
      pe.config = PERF_COUNT_HW_CACHE_REFERENCES;
      fd_cache_ref = perf_event_open(&pe, 0, -1, fd_cycles, 0);
      if (fd_cache_ref == -1) {
        perror("perf_event_open (cache references)");
        exit(1);
      }

      // ---------------------
      // member 3: cache misses
      // ---------------------
      pe.config = PERF_COUNT_HW_CACHE_MISSES;
      fd_cache_miss = perf_event_open(&pe, 0, -1, fd_cycles, 0);
      if (fd_cache_miss == -1) {
        perror("perf_event_open (cache misses)");
        exit(1);
      }

      // ---------------------
      // Enable the group
      // ---------------------
      ioctl(fd_cycles, PERF_EVENT_IOC_RESET, PERF_IOC_FLAG_GROUP);
      ioctl(fd_cycles, PERF_EVENT_IOC_ENABLE, PERF_IOC_FLAG_GROUP);

      // ======== Measured region ========

      sleeping_thread_to_gdb_regs(gdb_regs, p);

      // ======== End of measured region ========
      ioctl(fd_cycles, PERF_EVENT_IOC_DISABLE, PERF_IOC_FLAG_GROUP);

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
      if (f_csv == NULL) {
        perror("Error creating/opening CSV file");
        exit(1);
      }

      // Check if file is empty to add header
      fseek(f_csv, 0, SEEK_END);
      long size = ftell(f_csv);
      if (size == 0) {
        fprintf(f_csv, "function_name,cpu-cycles,instructions,cache-misses,"
                       "cache-ref newline");
      }

      // Write data
      fprintf(f_csv, "sleeping_thread_to_gdb_regs,%lu,%lu,%lu,%lu newline",
              count_cycles, count_instructions, count_cache_miss,
              count_cache_ref);

      fclose(f_csv);
    }
    free(gdb_regs);
    free(p);

    break;
  }

  // big-arr-10x
  case 1: {
    // static_instructions_O0 : 150
    // dynamic_instructions_O0 : 352
    // -------------------------------
    // static_instructions_O1 : 38
    // dynamic_instructions_O1 : 38
    // -------------------------------
    // static_instructions_O2 : 38
    // dynamic_instructions_O2 : 38
    // -------------------------------
    // static_instructions_O3 : 38
    // dynamic_instructions_O3 : 38
    // -------------------------------
    // static_instructions_Ofast : 38
    // dynamic_instructions_Ofast : 38
    // -------------------------------
    // static_instructions_Os : 38
    // dynamic_instructions_Os : 38
    // -------------------------------
    // static_instructions_Oz : 46
    // dynamic_instructions_Oz : 140
    // -------------------------------

    int _len_gdb_regs0 = 100;
    unsigned long *gdb_regs =
        (unsigned long *)malloc(_len_gdb_regs0 * sizeof(unsigned long));
    for (int _i0 = 0; _i0 < _len_gdb_regs0; _i0++) {
      gdb_regs[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
    }

    int _len_p0 = 100;
    struct task_struct *p =
        (struct task_struct *)malloc(_len_p0 * sizeof(struct task_struct));
    for (int _i0 = 0; _i0 < _len_p0; _i0++) {
      p[_i0].thread.reg31 = ((-2 * (next_i() % 2)) + 1) * next_i();
      p[_i0].thread.cp0_status = ((-2 * (next_i() % 2)) + 1) * next_i();
      p[_i0].thread.reg30 = ((-2 * (next_i() % 2)) + 1) * next_i();
      p[_i0].thread.reg29 = ((-2 * (next_i() % 2)) + 1) * next_i();
      p[_i0].thread.reg23 = ((-2 * (next_i() % 2)) + 1) * next_i();
      p[_i0].thread.reg22 = ((-2 * (next_i() % 2)) + 1) * next_i();
      p[_i0].thread.reg21 = ((-2 * (next_i() % 2)) + 1) * next_i();
      p[_i0].thread.reg20 = ((-2 * (next_i() % 2)) + 1) * next_i();
      p[_i0].thread.reg19 = ((-2 * (next_i() % 2)) + 1) * next_i();
      p[_i0].thread.reg18 = ((-2 * (next_i() % 2)) + 1) * next_i();
      p[_i0].thread.reg17 = ((-2 * (next_i() % 2)) + 1) * next_i();
      p[_i0].thread.reg16 = ((-2 * (next_i() % 2)) + 1) * next_i();
    }

    {

      struct perf_event_attr pe;
      int fd_cycles, fd_instructions, fd_cache_ref, fd_cache_miss;
      uint64_t count_cycles, count_instructions, count_cache_ref,
          count_cache_miss;

      // base event config
      memset(&pe, 0, sizeof(struct perf_event_attr));
      pe.size = sizeof(struct perf_event_attr);
      pe.disabled = 1;       // starts disabled
      pe.exclude_kernel = 0; // measure kernel too (0 = measure all)
      pe.exclude_hv = 1;     // ignore hypervisor

      // ---------------------
      // main group: cpu cycles
      // ---------------------
      pe.type = PERF_TYPE_HARDWARE;
      pe.config = PERF_COUNT_HW_CPU_CYCLES;
      fd_cycles = perf_event_open(&pe, 0, -1, -1, 0);
      if (fd_cycles == -1) {
        perror("perf_event_open (cycles)");
        exit(1);
      }

      // ---------------------
      // member 1: instructions
      // ---------------------
      pe.config = PERF_COUNT_HW_INSTRUCTIONS;
      fd_instructions = perf_event_open(&pe, 0, -1, fd_cycles, 0);
      if (fd_instructions == -1) {
        perror("perf_event_open (instructions)");
        exit(1);
      }

      // ---------------------
      // member 2: cache references
      // ---------------------
      pe.config = PERF_COUNT_HW_CACHE_REFERENCES;
      fd_cache_ref = perf_event_open(&pe, 0, -1, fd_cycles, 0);
      if (fd_cache_ref == -1) {
        perror("perf_event_open (cache references)");
        exit(1);
      }

      // ---------------------
      // member 3: cache misses
      // ---------------------
      pe.config = PERF_COUNT_HW_CACHE_MISSES;
      fd_cache_miss = perf_event_open(&pe, 0, -1, fd_cycles, 0);
      if (fd_cache_miss == -1) {
        perror("perf_event_open (cache misses)");
        exit(1);
      }

      // ---------------------
      // Enable the group
      // ---------------------
      ioctl(fd_cycles, PERF_EVENT_IOC_RESET, PERF_IOC_FLAG_GROUP);
      ioctl(fd_cycles, PERF_EVENT_IOC_ENABLE, PERF_IOC_FLAG_GROUP);

      // ======== Measured region ========

      sleeping_thread_to_gdb_regs(gdb_regs, p);

      // ======== End of measured region ========
      ioctl(fd_cycles, PERF_EVENT_IOC_DISABLE, PERF_IOC_FLAG_GROUP);

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
      if (f_csv == NULL) {
        perror("Error creating/opening CSV file");
        exit(1);
      }

      // Check if file is empty to add header
      fseek(f_csv, 0, SEEK_END);
      long size = ftell(f_csv);
      if (size == 0) {
        fprintf(f_csv, "function_name,cpu-cycles,instructions,cache-misses,"
                       "cache-ref newline");
      }

      // Write data
      fprintf(f_csv, "sleeping_thread_to_gdb_regs,%lu,%lu,%lu,%lu newline",
              count_cycles, count_instructions, count_cache_miss,
              count_cache_ref);

      fclose(f_csv);
    }
    free(gdb_regs);
    free(p);

    break;
  }

  default:
    usage();
    break;
  }

  return 0;
}
