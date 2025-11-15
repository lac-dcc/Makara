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
       2            empty\n\
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
typedef struct TYPE_5__ TYPE_2__;
typedef struct TYPE_4__ TYPE_1__;

/* Type definitions */
typedef int /*<<< orphan*/ WORD;
typedef int WCHAR;
struct TYPE_4__ {
  int /*<<< orphan*/ wgBlank;
  int /*<<< orphan*/ wgDefault;
};
struct TYPE_5__ {
  TYPE_1__ sfp;
};
typedef TYPE_2__ ScriptCache;
typedef int /*<<< orphan*/ SCRIPT_ANALYSIS;
typedef int INT;
typedef int /*<<< orphan*/ HDC;

/* Variables and functions */

__attribute__((used)) static void
ContextualShape_Control(HDC hdc, ScriptCache *psc, SCRIPT_ANALYSIS *psa,
                        WCHAR *pwcChars, INT cChars, WORD *pwOutGlyphs,
                        INT *pcGlyphs, INT cMaxGlyphs, WORD *pwLogClust) {
  int i;
  for (i = 0; i < cChars; i++) {
    switch (pwcChars[i]) {
    case 0x000A:
    case 0x000D:
      pwOutGlyphs[i] = psc->sfp.wgBlank;
      break;
    default:
      if (pwcChars[i] < 0x1C)
        pwOutGlyphs[i] = psc->sfp.wgDefault;
      else
        pwOutGlyphs[i] = psc->sfp.wgBlank;
    }
  }
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
    int hdc = 255;

    int cChars = 255;

    int cMaxGlyphs = 255;

    int _len_psc0 = 65025;
    struct TYPE_5__ *psc =
        (struct TYPE_5__ *)malloc(_len_psc0 * sizeof(struct TYPE_5__));
    for (int _i0 = 0; _i0 < _len_psc0; _i0++) {
      psc[_i0].sfp.wgBlank = ((-2 * (next_i() % 2)) + 1) * next_i();
      psc[_i0].sfp.wgDefault = ((-2 * (next_i() % 2)) + 1) * next_i();
    }

    int _len_psa0 = 65025;
    int *psa = (int *)malloc(_len_psa0 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_psa0; _i0++) {
      psa[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
    }

    int _len_pwcChars0 = 65025;
    int *pwcChars = (int *)malloc(_len_pwcChars0 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_pwcChars0; _i0++) {
      pwcChars[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
    }

    int _len_pwOutGlyphs0 = 65025;
    int *pwOutGlyphs = (int *)malloc(_len_pwOutGlyphs0 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_pwOutGlyphs0; _i0++) {
      pwOutGlyphs[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
    }

    int _len_pcGlyphs0 = 65025;
    int *pcGlyphs = (int *)malloc(_len_pcGlyphs0 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_pcGlyphs0; _i0++) {
      pcGlyphs[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
    }

    int _len_pwLogClust0 = 65025;
    int *pwLogClust = (int *)malloc(_len_pwLogClust0 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_pwLogClust0; _i0++) {
      pwLogClust[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
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

      ContextualShape_Control(hdc, psc, psa, pwcChars, cChars, pwOutGlyphs,
                              pcGlyphs, cMaxGlyphs, pwLogClust);

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
      fprintf(f_csv, "ContextualShape_Control,%lu,%lu,%lu,%lu newline",
              count_cycles, count_instructions, count_cache_miss,
              count_cache_ref);

      fclose(f_csv);
    }
    free(psc);
    free(psa);
    free(pwcChars);
    free(pwOutGlyphs);
    free(pcGlyphs);
    free(pwLogClust);

    break;
  }
  // big-arr-10x
  case 1: {
    int hdc = 10;

    int cChars = 10;

    int cMaxGlyphs = 10;

    int _len_psc0 = 100;
    struct TYPE_5__ *psc =
        (struct TYPE_5__ *)malloc(_len_psc0 * sizeof(struct TYPE_5__));
    for (int _i0 = 0; _i0 < _len_psc0; _i0++) {
      psc[_i0].sfp.wgBlank = ((-2 * (next_i() % 2)) + 1) * next_i();
      psc[_i0].sfp.wgDefault = ((-2 * (next_i() % 2)) + 1) * next_i();
    }

    int _len_psa0 = 100;
    int *psa = (int *)malloc(_len_psa0 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_psa0; _i0++) {
      psa[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
    }

    int _len_pwcChars0 = 100;
    int *pwcChars = (int *)malloc(_len_pwcChars0 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_pwcChars0; _i0++) {
      pwcChars[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
    }

    int _len_pwOutGlyphs0 = 100;
    int *pwOutGlyphs = (int *)malloc(_len_pwOutGlyphs0 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_pwOutGlyphs0; _i0++) {
      pwOutGlyphs[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
    }

    int _len_pcGlyphs0 = 100;
    int *pcGlyphs = (int *)malloc(_len_pcGlyphs0 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_pcGlyphs0; _i0++) {
      pcGlyphs[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
    }

    int _len_pwLogClust0 = 100;
    int *pwLogClust = (int *)malloc(_len_pwLogClust0 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_pwLogClust0; _i0++) {
      pwLogClust[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
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

      ContextualShape_Control(hdc, psc, psa, pwcChars, cChars, pwOutGlyphs,
                              pcGlyphs, cMaxGlyphs, pwLogClust);

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
      fprintf(f_csv, "ContextualShape_Control,%lu,%lu,%lu,%lu newline",
              count_cycles, count_instructions, count_cache_miss,
              count_cache_ref);

      fclose(f_csv);
    }
    free(psc);
    free(psa);
    free(pwcChars);
    free(pwOutGlyphs);
    free(pcGlyphs);
    free(pwLogClust);

    break;
  }
  // empty
  case 2: {
    int hdc = ((-2 * (next_i() % 2)) + 1) * next_i();

    int cChars = ((-2 * (next_i() % 2)) + 1) * next_i();

    int cMaxGlyphs = ((-2 * (next_i() % 2)) + 1) * next_i();

    int _len_psc0 = 1;
    struct TYPE_5__ *psc =
        (struct TYPE_5__ *)malloc(_len_psc0 * sizeof(struct TYPE_5__));
    for (int _i0 = 0; _i0 < _len_psc0; _i0++) {
      psc[_i0].sfp.wgBlank = ((-2 * (next_i() % 2)) + 1) * next_i();
      psc[_i0].sfp.wgDefault = ((-2 * (next_i() % 2)) + 1) * next_i();
    }

    int _len_psa0 = 1;
    int *psa = (int *)malloc(_len_psa0 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_psa0; _i0++) {
      psa[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
    }

    int _len_pwcChars0 = 1;
    int *pwcChars = (int *)malloc(_len_pwcChars0 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_pwcChars0; _i0++) {
      pwcChars[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
    }

    int _len_pwOutGlyphs0 = 1;
    int *pwOutGlyphs = (int *)malloc(_len_pwOutGlyphs0 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_pwOutGlyphs0; _i0++) {
      pwOutGlyphs[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
    }

    int _len_pcGlyphs0 = 1;
    int *pcGlyphs = (int *)malloc(_len_pcGlyphs0 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_pcGlyphs0; _i0++) {
      pcGlyphs[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
    }

    int _len_pwLogClust0 = 1;
    int *pwLogClust = (int *)malloc(_len_pwLogClust0 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_pwLogClust0; _i0++) {
      pwLogClust[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
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

      ContextualShape_Control(hdc, psc, psa, pwcChars, cChars, pwOutGlyphs,
                              pcGlyphs, cMaxGlyphs, pwLogClust);

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
      fprintf(f_csv, "ContextualShape_Control,%lu,%lu,%lu,%lu newline",
              count_cycles, count_instructions, count_cache_miss,
              count_cache_ref);

      fclose(f_csv);
    }
    free(psc);
    free(psa);
    free(pwcChars);
    free(pwOutGlyphs);
    free(pcGlyphs);
    free(pwLogClust);

    break;
  }
  default:
    usage();
    break;
  }

  return 0;
}
