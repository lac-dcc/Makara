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
       1            big-arr\n\
       2            big-arr-10x\n\
       3            empty\n\
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
typedef int u16;

/* Variables and functions */
int CEC_PHYS_ADDR_INVALID;
int EINVAL;

int cec_phys_addr_validate(u16 phys_addr, u16 *parent, u16 *port) {
  int i;

  if (parent)
    *parent = phys_addr;
  if (port)
    *port = 0;
  if (phys_addr == CEC_PHYS_ADDR_INVALID)
    return 0;
  for (i = 0; i < 16; i += 4)
    if (phys_addr & (0xf << i))
      break;
  if (i == 16)
    return 0;
  if (parent)
    *parent = phys_addr & (0xfff0 << i);
  if (port)
    *port = (phys_addr >> i) & 0xf;
  for (i += 4; i < 16; i += 4)
    if ((phys_addr & (0xf << i)) == 0)
      return -EINVAL;
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

  // int-bounds
  case 0: {
    // static_instructions_O0 : 76
    // dynamic_instructions_O0 : 86
    // -------------------------------
    // static_instructions_O1 : 49
    // dynamic_instructions_O1 : 58
    // -------------------------------
    // static_instructions_O2 : 46
    // dynamic_instructions_O2 : 46
    // -------------------------------
    // static_instructions_O3 : 46
    // dynamic_instructions_O3 : 46
    // -------------------------------
    // static_instructions_Ofast : 46
    // dynamic_instructions_Ofast : 46
    // -------------------------------
    // static_instructions_Os : 40
    // dynamic_instructions_Os : 48
    // -------------------------------
    // static_instructions_Oz : 43
    // dynamic_instructions_Oz : 51
    // -------------------------------

    int phys_addr = 100;

    int _len_parent0 = 1;
    int *parent = (int *)malloc(_len_parent0 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_parent0; _i0++) {
      parent[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
    }

    int _len_port0 = 1;
    int *port = (int *)malloc(_len_port0 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_port0; _i0++) {
      port[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
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

      __typeof__(cec_phys_addr_validate(phys_addr, parent, port)) __perf_ret =
          cec_phys_addr_validate(phys_addr, parent, port);

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
      fprintf(f_csv, "cec_phys_addr_validate,%lu,%lu,%lu,%lu newline",
              count_cycles, count_instructions, count_cache_miss,
              count_cache_ref);

      fclose(f_csv);

      __perf_ret;
    });
    printf("%d\n", benchRet);
    free(parent);
    free(port);

    break;
  }

  // big-arr
  case 1: {
    // static_instructions_O0 : 76
    // dynamic_instructions_O0 : 86
    // -------------------------------
    // static_instructions_O1 : 49
    // dynamic_instructions_O1 : 58
    // -------------------------------
    // static_instructions_O2 : 46
    // dynamic_instructions_O2 : 46
    // -------------------------------
    // static_instructions_O3 : 46
    // dynamic_instructions_O3 : 46
    // -------------------------------
    // static_instructions_Ofast : 46
    // dynamic_instructions_Ofast : 46
    // -------------------------------
    // static_instructions_Os : 40
    // dynamic_instructions_Os : 48
    // -------------------------------
    // static_instructions_Oz : 43
    // dynamic_instructions_Oz : 51
    // -------------------------------

    int phys_addr = 255;

    int _len_parent0 = 65025;
    int *parent = (int *)malloc(_len_parent0 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_parent0; _i0++) {
      parent[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
    }

    int _len_port0 = 65025;
    int *port = (int *)malloc(_len_port0 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_port0; _i0++) {
      port[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
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

      __typeof__(cec_phys_addr_validate(phys_addr, parent, port)) __perf_ret =
          cec_phys_addr_validate(phys_addr, parent, port);

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
      fprintf(f_csv, "cec_phys_addr_validate,%lu,%lu,%lu,%lu newline",
              count_cycles, count_instructions, count_cache_miss,
              count_cache_ref);

      fclose(f_csv);

      __perf_ret;
    });
    printf("%d\n", benchRet);
    free(parent);
    free(port);

    break;
  }

  // big-arr-10x
  case 2: {
    // static_instructions_O0 : 71
    // dynamic_instructions_O0 : 71
    // -------------------------------
    // static_instructions_O1 : 49
    // dynamic_instructions_O1 : 49
    // -------------------------------
    // static_instructions_O2 : 40
    // dynamic_instructions_O2 : 40
    // -------------------------------
    // static_instructions_O3 : 40
    // dynamic_instructions_O3 : 40
    // -------------------------------
    // static_instructions_Ofast : 40
    // dynamic_instructions_Ofast : 40
    // -------------------------------
    // static_instructions_Os : 40
    // dynamic_instructions_Os : 40
    // -------------------------------
    // static_instructions_Oz : 43
    // dynamic_instructions_Oz : 43
    // -------------------------------

    int phys_addr = 10;

    int _len_parent0 = 100;
    int *parent = (int *)malloc(_len_parent0 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_parent0; _i0++) {
      parent[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
    }

    int _len_port0 = 100;
    int *port = (int *)malloc(_len_port0 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_port0; _i0++) {
      port[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
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

      __typeof__(cec_phys_addr_validate(phys_addr, parent, port)) __perf_ret =
          cec_phys_addr_validate(phys_addr, parent, port);

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
      fprintf(f_csv, "cec_phys_addr_validate,%lu,%lu,%lu,%lu newline",
              count_cycles, count_instructions, count_cache_miss,
              count_cache_ref);

      fclose(f_csv);

      __perf_ret;
    });
    printf("%d\n", benchRet);
    free(parent);
    free(port);

    break;
  }

  // empty
  case 3: {
    // static_instructions_O0 : 73
    // dynamic_instructions_O0 : 105
    // -------------------------------
    // static_instructions_O1 : 47
    // dynamic_instructions_O1 : 67
    // -------------------------------
    // static_instructions_O2 : 49
    // dynamic_instructions_O2 : 49
    // -------------------------------
    // static_instructions_O3 : 49
    // dynamic_instructions_O3 : 49
    // -------------------------------
    // static_instructions_Ofast : 49
    // dynamic_instructions_Ofast : 49
    // -------------------------------
    // static_instructions_Os : 40
    // dynamic_instructions_Os : 58
    // -------------------------------
    // static_instructions_Oz : 41
    // dynamic_instructions_Oz : 59
    // -------------------------------

    int phys_addr = ((-2 * (next_i() % 2)) + 1) * next_i();

    int _len_parent0 = 1;
    int *parent = (int *)malloc(_len_parent0 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_parent0; _i0++) {
      parent[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
    }

    int _len_port0 = 1;
    int *port = (int *)malloc(_len_port0 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_port0; _i0++) {
      port[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
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

      __typeof__(cec_phys_addr_validate(phys_addr, parent, port)) __perf_ret =
          cec_phys_addr_validate(phys_addr, parent, port);

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
      fprintf(f_csv, "cec_phys_addr_validate,%lu,%lu,%lu,%lu newline",
              count_cycles, count_instructions, count_cache_miss,
              count_cache_ref);

      fclose(f_csv);

      __perf_ret;
    });
    printf("%d\n", benchRet);
    free(parent);
    free(port);

    break;
  }

  default:
    usage();
    break;
  }

  return 0;
}
