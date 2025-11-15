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

/* Type definitions */
typedef scalar_t__ u32;
struct dsi_isr_data {
  void *arg;
  scalar_t__ mask;
  int /*<<< orphan*/ *isr;
};
typedef int /*<<< orphan*/ *omap_dsi_isr_t;

/* Variables and functions */
int EINVAL;

__attribute__((used)) static int
_dsi_unregister_isr(omap_dsi_isr_t isr, void *arg, u32 mask,
                    struct dsi_isr_data *isr_array, unsigned isr_array_size) {
  struct dsi_isr_data *isr_data;
  int i;

  for (i = 0; i < isr_array_size; i++) {
    isr_data = &isr_array[i];
    if (isr_data->isr != isr || isr_data->arg != arg || isr_data->mask != mask)
      continue;

    isr_data->isr = NULL;
    isr_data->arg = NULL;
    isr_data->mask = 0;

    return 0;
  }

  return -EINVAL;
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
    long mask = 255;

    unsigned int isr_array_size = 255;

    int _len_isr0 = 65025;
    int *isr = (int *)malloc(_len_isr0 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_isr0; _i0++) {
      isr[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
    }

    void *arg;

    int _len_isr_array0 = 65025;
    struct dsi_isr_data *isr_array = (struct dsi_isr_data *)malloc(
        _len_isr_array0 * sizeof(struct dsi_isr_data));
    for (int _i0 = 0; _i0 < _len_isr_array0; _i0++) {
      isr_array[_i0].mask = ((-2 * (next_i() % 2)) + 1) * next_i();
      int _len_isr_array__i0__isr0 = 1;
      isr_array[_i0].isr =
          (int *)malloc(_len_isr_array__i0__isr0 * sizeof(int));
      for (int _j0 = 0; _j0 < _len_isr_array__i0__isr0; _j0++) {
        isr_array[_i0].isr[_j0] = ((-2 * (next_i() % 2)) + 1) * next_i();
      }
    }

    int benchRet = ({
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

      __typeof__(_dsi_unregister_isr(isr, arg, mask, isr_array,
                                     isr_array_size)) __perf_ret =
          _dsi_unregister_isr(isr, arg, mask, isr_array, isr_array_size);

      // ======== End of measured region ========

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
      fprintf(f_csv, "_dsi_unregister_isr,%lu,%lu,%lu,%lu newline",
              count_cycles, count_instructions, count_cache_miss,
              count_cache_ref);

      fclose(f_csv);

      __perf_ret;
    });
    printf("%d\n", benchRet);
    free(isr);
    for (int _aux = 0; _aux < _len_isr_array0; _aux++) {
      free(isr_array[_aux].isr);
    }
    free(isr_array);

    break;
  }
  // big-arr-10x
  case 1: {
    long mask = 10;

    unsigned int isr_array_size = 10;

    int _len_isr0 = 100;
    int *isr = (int *)malloc(_len_isr0 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_isr0; _i0++) {
      isr[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
    }

    void *arg;

    int _len_isr_array0 = 100;
    struct dsi_isr_data *isr_array = (struct dsi_isr_data *)malloc(
        _len_isr_array0 * sizeof(struct dsi_isr_data));
    for (int _i0 = 0; _i0 < _len_isr_array0; _i0++) {
      isr_array[_i0].mask = ((-2 * (next_i() % 2)) + 1) * next_i();
      int _len_isr_array__i0__isr0 = 1;
      isr_array[_i0].isr =
          (int *)malloc(_len_isr_array__i0__isr0 * sizeof(int));
      for (int _j0 = 0; _j0 < _len_isr_array__i0__isr0; _j0++) {
        isr_array[_i0].isr[_j0] = ((-2 * (next_i() % 2)) + 1) * next_i();
      }
    }

    int benchRet = ({
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

      __typeof__(_dsi_unregister_isr(isr, arg, mask, isr_array,
                                     isr_array_size)) __perf_ret =
          _dsi_unregister_isr(isr, arg, mask, isr_array, isr_array_size);

      // ======== End of measured region ========

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
      fprintf(f_csv, "_dsi_unregister_isr,%lu,%lu,%lu,%lu newline",
              count_cycles, count_instructions, count_cache_miss,
              count_cache_ref);

      fclose(f_csv);

      __perf_ret;
    });
    printf("%d\n", benchRet);
    free(isr);
    for (int _aux = 0; _aux < _len_isr_array0; _aux++) {
      free(isr_array[_aux].isr);
    }
    free(isr_array);

    break;
  }
  default:
    usage();
    break;
  }

  return 0;
}
