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
typedef struct TYPE_3__ TYPE_1__;

/* Type definitions */
typedef int /*<<< orphan*/ bpb_tbl;
typedef scalar_t__ USHORT;
struct TYPE_3__ {
  int member_1;
  int member_2;
  int member_3;
  int member_4;
  int member_5;
  int member_6;
  int member_7;
  int member_8;
  int member_9;
  scalar_t__ SmallSectors;
  int /*<<< orphan*/ member_11;
  int /*<<< orphan*/ member_10;
  int /*<<< orphan*/ const member_0;
};
typedef TYPE_1__ DOS_BPB;

/* Variables and functions */
#define VFD_BYTES_PER_SECTOR 128

__attribute__((used)) static const DOS_BPB *SelectDosBpb(USHORT nSectors) {
  static const DOS_BPB bpb_tbl[] = {
      // b/s				  s/c r fat root  sec  desc s/f s/t  h
      {VFD_BYTES_PER_SECTOR, 2, 1, 2, 112, 320, 0xFE, 1, 8, 1, 0,
       0}, // 160KB 5.25"
      {VFD_BYTES_PER_SECTOR, 2, 1, 2, 112, 360, 0xFC, 1, 9, 1, 0,
       0}, // 180KB 5.25"
      {VFD_BYTES_PER_SECTOR, 2, 1, 2, 112, 640, 0xFF, 1, 8, 2, 0,
       0}, // 320KB 5.25"
      {VFD_BYTES_PER_SECTOR, 2, 1, 2, 112, 720, 0xFD, 2, 9, 2, 0,
       0}, // 360KB 5.25"
      {VFD_BYTES_PER_SECTOR, 2, 1, 2, 112, 1280, 0xFB, 2, 8, 2, 0,
       0}, // 640KB 5.25" / 3.5"
      {VFD_BYTES_PER_SECTOR, 2, 1, 2, 112, 1440, 0xF9, 3, 9, 2, 0,
       0}, // 720KB 5.25" / 3.5"
      {VFD_BYTES_PER_SECTOR, 2, 1, 2, 112, 1640, 0xF9, 3, 10, 2, 0,
       0}, // 820KB 3.5"
      {VFD_BYTES_PER_SECTOR, 1, 1, 2, 224, 2400, 0xF9, 7, 15, 2, 0,
       0}, // 1.20MB 5.25" / 3.5"
      {VFD_BYTES_PER_SECTOR, 1, 1, 2, 224, 2880, 0xF0, 9, 18, 2, 0,
       0}, // 1.44MB 3.5"
      {VFD_BYTES_PER_SECTOR, 1, 1, 2, 224, 3360, 0xF0, 10, 21, 2, 0,
       0}, // 1.68MB 3.5"
      {VFD_BYTES_PER_SECTOR, 1, 1, 2, 224, 3444, 0xF0, 10, 21, 2, 0,
       0}, // 1.72MB 3.5"
      {VFD_BYTES_PER_SECTOR, 2, 1, 2, 240, 5760, 0xF0, 9, 36, 2, 0,
       0}, // 2.88MB 3.5"
  };

  int i;

  for (i = 0; i < sizeof(bpb_tbl) / sizeof(bpb_tbl[0]); i++) {
    if (nSectors == bpb_tbl[i].SmallSectors) {
      return &bpb_tbl[i];
    }
  }

  return NULL;
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
    long nSectors = 100;
    const struct TYPE_3__ *benchRet = ({
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

      __typeof__(SelectDosBpb(nSectors)) __perf_ret = SelectDosBpb(nSectors);

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
      fprintf(f_csv, "SelectDosBpb,%lu,%lu,%lu,%lu newline", count_cycles,
              count_instructions, count_cache_miss, count_cache_ref);

      fclose(f_csv);

      __perf_ret;
    });

    break;
  }
  // big-arr
  case 1: {
    long nSectors = 255;
    const struct TYPE_3__ *benchRet = ({
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

      __typeof__(SelectDosBpb(nSectors)) __perf_ret = SelectDosBpb(nSectors);

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
      fprintf(f_csv, "SelectDosBpb,%lu,%lu,%lu,%lu newline", count_cycles,
              count_instructions, count_cache_miss, count_cache_ref);

      fclose(f_csv);

      __perf_ret;
    });

    break;
  }
  // big-arr-10x
  case 2: {
    long nSectors = 10;
    const struct TYPE_3__ *benchRet = ({
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

      __typeof__(SelectDosBpb(nSectors)) __perf_ret = SelectDosBpb(nSectors);

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
      fprintf(f_csv, "SelectDosBpb,%lu,%lu,%lu,%lu newline", count_cycles,
              count_instructions, count_cache_miss, count_cache_ref);

      fclose(f_csv);

      __perf_ret;
    });

    break;
  }

  default:
    usage();
    break;
  }

  return 0;
}
