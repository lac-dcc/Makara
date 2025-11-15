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

/* Type definitions */
typedef scalar_t__ tre_tag_direction_t;

/* Variables and functions */
scalar_t__ TRE_TAG_MINIMIZE;

__attribute__((used)) static int
tre_tag_order(int num_tags, tre_tag_direction_t *tag_directions, int *t1,
              int *t2) {
  int i;
  for (i = 0; i < num_tags; i++) {
    if (tag_directions[i] == TRE_TAG_MINIMIZE) {
      if (t1[i] < t2[i])
        return 1;
      if (t1[i] > t2[i])
        return 0;
    } else {
      if (t1[i] > t2[i])
        return 1;
      if (t1[i] < t2[i])
        return 0;
    }
  }
  /*  assert(0);*/
  return 0;
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
    // static_instructions_O0 : 39
    // dynamic_instructions_O0 : 7154
    // -------------------------------
    // static_instructions_O1 : 22
    // dynamic_instructions_O1 : 2816
    // -------------------------------
    // static_instructions_O2 : 22
    // dynamic_instructions_O2 : 2816
    // -------------------------------
    // static_instructions_O3 : 22
    // dynamic_instructions_O3 : 2816
    // -------------------------------
    // static_instructions_Ofast : 22
    // dynamic_instructions_Ofast : 2816
    // -------------------------------
    // static_instructions_Os : 22
    // dynamic_instructions_Os : 2816
    // -------------------------------
    // static_instructions_Oz : 23
    // dynamic_instructions_Oz : 2819
    // -------------------------------

    int num_tags = 255;

    int _len_tag_directions0 = 65025;
    long *tag_directions = (long *)malloc(_len_tag_directions0 * sizeof(long));
    for (int _i0 = 0; _i0 < _len_tag_directions0; _i0++) {
      tag_directions[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
    }

    int _len_t10 = 65025;
    int *t1 = (int *)malloc(_len_t10 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_t10; _i0++) {
      t1[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
    }

    int _len_t20 = 65025;
    int *t2 = (int *)malloc(_len_t20 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_t20; _i0++) {
      t2[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
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

      __typeof__(tre_tag_order(num_tags, tag_directions, t1, t2)) __perf_ret =
          tre_tag_order(num_tags, tag_directions, t1, t2);

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
      fprintf(f_csv, "tre_tag_order,%lu,%lu,%lu,%lu newline", count_cycles,
              count_instructions, count_cache_miss, count_cache_ref);

      fclose(f_csv);

      __perf_ret;
    });
    printf("%d\n", benchRet);
    free(tag_directions);
    free(t1);
    free(t2);

    break;
  }

  // big-arr-10x
  case 1: {
    // static_instructions_O0 : 39
    // dynamic_instructions_O0 : 294
    // -------------------------------
    // static_instructions_O1 : 22
    // dynamic_instructions_O1 : 121
    // -------------------------------
    // static_instructions_O2 : 22
    // dynamic_instructions_O2 : 121
    // -------------------------------
    // static_instructions_O3 : 22
    // dynamic_instructions_O3 : 121
    // -------------------------------
    // static_instructions_Ofast : 22
    // dynamic_instructions_Ofast : 121
    // -------------------------------
    // static_instructions_Os : 22
    // dynamic_instructions_Os : 121
    // -------------------------------
    // static_instructions_Oz : 23
    // dynamic_instructions_Oz : 124
    // -------------------------------

    int num_tags = 10;

    int _len_tag_directions0 = 100;
    long *tag_directions = (long *)malloc(_len_tag_directions0 * sizeof(long));
    for (int _i0 = 0; _i0 < _len_tag_directions0; _i0++) {
      tag_directions[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
    }

    int _len_t10 = 100;
    int *t1 = (int *)malloc(_len_t10 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_t10; _i0++) {
      t1[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
    }

    int _len_t20 = 100;
    int *t2 = (int *)malloc(_len_t20 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_t20; _i0++) {
      t2[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
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

      __typeof__(tre_tag_order(num_tags, tag_directions, t1, t2)) __perf_ret =
          tre_tag_order(num_tags, tag_directions, t1, t2);

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
      fprintf(f_csv, "tre_tag_order,%lu,%lu,%lu,%lu newline", count_cycles,
              count_instructions, count_cache_miss, count_cache_ref);

      fclose(f_csv);

      __perf_ret;
    });
    printf("%d\n", benchRet);
    free(tag_directions);
    free(t1);
    free(t2);

    break;
  }

  // empty
  case 2: {
    // static_instructions_O0 : 14
    // dynamic_instructions_O0 : 14
    // -------------------------------
    // static_instructions_O1 : 7
    // dynamic_instructions_O1 : 7
    // -------------------------------
    // static_instructions_O2 : 7
    // dynamic_instructions_O2 : 7
    // -------------------------------
    // static_instructions_O3 : 7
    // dynamic_instructions_O3 : 7
    // -------------------------------
    // static_instructions_Ofast : 7
    // dynamic_instructions_Ofast : 7
    // -------------------------------
    // static_instructions_Os : 8
    // dynamic_instructions_Os : 8
    // -------------------------------
    // static_instructions_Oz : 14
    // dynamic_instructions_Oz : 14
    // -------------------------------

    int num_tags = ((-2 * (next_i() % 2)) + 1) * next_i();

    int _len_tag_directions0 = 1;
    long *tag_directions = (long *)malloc(_len_tag_directions0 * sizeof(long));
    for (int _i0 = 0; _i0 < _len_tag_directions0; _i0++) {
      tag_directions[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
    }

    int _len_t10 = 1;
    int *t1 = (int *)malloc(_len_t10 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_t10; _i0++) {
      t1[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
    }

    int _len_t20 = 1;
    int *t2 = (int *)malloc(_len_t20 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_t20; _i0++) {
      t2[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
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

      __typeof__(tre_tag_order(num_tags, tag_directions, t1, t2)) __perf_ret =
          tre_tag_order(num_tags, tag_directions, t1, t2);

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
      fprintf(f_csv, "tre_tag_order,%lu,%lu,%lu,%lu newline", count_cycles,
              count_instructions, count_cache_miss, count_cache_ref);

      fclose(f_csv);

      __perf_ret;
    });
    printf("%d\n", benchRet);
    free(tag_directions);
    free(t1);
    free(t2);

    break;
  }

  default:
    usage();
    break;
  }

  return 0;
}
