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
typedef scalar_t__ UChar;
typedef scalar_t__ Int32;

/* Variables and functions */

void BZ2_hbAssignCodes(Int32 *code, UChar *length, Int32 minLen, Int32 maxLen,
                       Int32 alphaSize) {
  Int32 n, vec, i;

  vec = 0;
  for (n = minLen; n <= maxLen; n++) {
    for (i = 0; i < alphaSize; i++)
      if (length[i] == n) {
        code[i] = vec;
        vec++;
      };
    vec <<= 1;
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
    // static_instructions_O0 : 36
    // dynamic_instructions_O0 : 3344
    // -------------------------------
    // static_instructions_O1 : 22
    // dynamic_instructions_O1 : 1292
    // -------------------------------
    // static_instructions_O2 : 31
    // dynamic_instructions_O2 : 913
    // -------------------------------
    // static_instructions_O3 : 31
    // dynamic_instructions_O3 : 913
    // -------------------------------
    // static_instructions_Ofast : 31
    // dynamic_instructions_Ofast : 913
    // -------------------------------
    // static_instructions_Os : 20
    // dynamic_instructions_Os : 1290
    // -------------------------------
    // static_instructions_Oz : 19
    // dynamic_instructions_Oz : 1547
    // -------------------------------

    long minLen = 255;

    long maxLen = 255;

    long alphaSize = 255;

    int _len_code0 = 65025;
    long *code = (long *)malloc(_len_code0 * sizeof(long));
    for (int _i0 = 0; _i0 < _len_code0; _i0++) {
      code[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
    }

    int _len_length0 = 65025;
    long *length = (long *)malloc(_len_length0 * sizeof(long));
    for (int _i0 = 0; _i0 < _len_length0; _i0++) {
      length[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
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

      BZ2_hbAssignCodes(code, length, minLen, maxLen, alphaSize);

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
      fprintf(f_csv, "BZ2_hbAssignCodes,%lu,%lu,%lu,%lu newline", count_cycles,
              count_instructions, count_cache_miss, count_cache_ref);

      fclose(f_csv);
    }
    free(code);
    free(length);

    break;
  }

  // big-arr-10x
  case 1: {
    // static_instructions_O0 : 36
    // dynamic_instructions_O0 : 159
    // -------------------------------
    // static_instructions_O1 : 22
    // dynamic_instructions_O1 : 67
    // -------------------------------
    // static_instructions_O2 : 29
    // dynamic_instructions_O2 : 57
    // -------------------------------
    // static_instructions_O3 : 29
    // dynamic_instructions_O3 : 57
    // -------------------------------
    // static_instructions_Ofast : 29
    // dynamic_instructions_Ofast : 57
    // -------------------------------
    // static_instructions_Os : 20
    // dynamic_instructions_Os : 65
    // -------------------------------
    // static_instructions_Oz : 19
    // dynamic_instructions_Oz : 77
    // -------------------------------

    long minLen = 10;

    long maxLen = 10;

    long alphaSize = 10;

    int _len_code0 = 100;
    long *code = (long *)malloc(_len_code0 * sizeof(long));
    for (int _i0 = 0; _i0 < _len_code0; _i0++) {
      code[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
    }

    int _len_length0 = 100;
    long *length = (long *)malloc(_len_length0 * sizeof(long));
    for (int _i0 = 0; _i0 < _len_length0; _i0++) {
      length[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
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

      BZ2_hbAssignCodes(code, length, minLen, maxLen, alphaSize);

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
      fprintf(f_csv, "BZ2_hbAssignCodes,%lu,%lu,%lu,%lu newline", count_cycles,
              count_instructions, count_cache_miss, count_cache_ref);

      fclose(f_csv);
    }
    free(code);
    free(length);

    break;
  }

  // empty
  case 2: {
    // static_instructions_O0 : 26
    // dynamic_instructions_O0 : 29
    // -------------------------------
    // static_instructions_O1 : 15
    // dynamic_instructions_O1 : 15
    // -------------------------------
    // static_instructions_O2 : 17
    // dynamic_instructions_O2 : 17
    // -------------------------------
    // static_instructions_O3 : 8
    // dynamic_instructions_O3 : 8
    // -------------------------------
    // static_instructions_Ofast : 8
    // dynamic_instructions_Ofast : 8
    // -------------------------------
    // static_instructions_Os : 14
    // dynamic_instructions_Os : 14
    // -------------------------------
    // static_instructions_Oz : 15
    // dynamic_instructions_Oz : 17
    // -------------------------------

    long minLen = ((-2 * (next_i() % 2)) + 1) * next_i();

    long maxLen = ((-2 * (next_i() % 2)) + 1) * next_i();

    long alphaSize = ((-2 * (next_i() % 2)) + 1) * next_i();

    int _len_code0 = 1;
    long *code = (long *)malloc(_len_code0 * sizeof(long));
    for (int _i0 = 0; _i0 < _len_code0; _i0++) {
      code[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
    }

    int _len_length0 = 1;
    long *length = (long *)malloc(_len_length0 * sizeof(long));
    for (int _i0 = 0; _i0 < _len_length0; _i0++) {
      length[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
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

      BZ2_hbAssignCodes(code, length, minLen, maxLen, alphaSize);

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
      fprintf(f_csv, "BZ2_hbAssignCodes,%lu,%lu,%lu,%lu newline", count_cycles,
              count_instructions, count_cache_miss, count_cache_ref);

      fclose(f_csv);
    }
    free(code);
    free(length);

    break;
  }

  default:
    usage();
    break;
  }

  return 0;
}
