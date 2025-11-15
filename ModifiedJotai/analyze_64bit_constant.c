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
       0            int-bounds\n\
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

/* Type definitions */
typedef int u32;

/* Variables and functions */

__attribute__((used)) static void analyze_64bit_constant(u32 high_bits,
                                                         u32 low_bits,
                                                         int *hbsp, int *lbsp,
                                                         int *abbasp) {
  int lowest_bit_set, highest_bit_set, all_bits_between_are_set;
  int i;

  lowest_bit_set = highest_bit_set = -1;
  i = 0;
  do {
    if ((lowest_bit_set == -1) && ((low_bits >> i) & 1))
      lowest_bit_set = i;
    if ((highest_bit_set == -1) && ((high_bits >> (32 - i - 1)) & 1))
      highest_bit_set = (64 - i - 1);
  } while (++i < 32 && (highest_bit_set == -1 || lowest_bit_set == -1));
  if (i == 32) {
    i = 0;
    do {
      if (lowest_bit_set == -1 && ((high_bits >> i) & 1))
        lowest_bit_set = i + 32;
      if (highest_bit_set == -1 && ((low_bits >> (32 - i - 1)) & 1))
        highest_bit_set = 32 - i - 1;
    } while (++i < 32 && (highest_bit_set == -1 || lowest_bit_set == -1));
  }

  all_bits_between_are_set = 1;
  for (i = lowest_bit_set; i <= highest_bit_set; i++) {
    if (i < 32) {
      if ((low_bits & (1 << i)) != 0)
        continue;
    } else {
      if ((high_bits & (1 << (i - 32))) != 0)
        continue;
    }
    all_bits_between_are_set = 0;
    break;
  }
  *hbsp = highest_bit_set;
  *lbsp = lowest_bit_set;
  *abbasp = all_bits_between_are_set;
}

// ------------------------------------------------------------------------- //

// ------------------------------------------------------------------------- //

int main(int argc, char *argv[]) {

  if (argc != 2) {
    usage();
    return 1;
  }

  int opt = atoi(argv[1]);
  switch (opt) {

  // int-bounds
  case 0: {
    int high_bits = 100;
    int low_bits = 100;
    int _len_hbsp0 = 1;
    int *hbsp = (int *)malloc(_len_hbsp0 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_hbsp0; _i0++) {
      hbsp[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
    }
    int _len_lbsp0 = 1;
    int *lbsp = (int *)malloc(_len_lbsp0 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_lbsp0; _i0++) {
      lbsp[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
    }
    int _len_abbasp0 = 1;
    int *abbasp = (int *)malloc(_len_abbasp0 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_abbasp0; _i0++) {
      abbasp[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
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

      analyze_64bit_constant(high_bits, low_bits, hbsp, lbsp, abbasp);

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
      fprintf(f_csv, "analyze_64bit_constant,%lu,%lu,%lu,%lu newline",
              count_cycles, count_instructions, count_cache_miss,
              count_cache_ref);

      fclose(f_csv);
    }
    free(hbsp);
    free(lbsp);
    free(abbasp);

    break;
  }

  default:
    usage();
    break;
  }

  return 0;
}
