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
typedef int u8;

/* Variables and functions */

__attribute__((used)) static void construct_ctr_preload(u8 *ctr_preload,
                                                        int a4_exists,
                                                        int qc_exists, u8 *mpdu,
                                                        u8 *pn_vector, int c) {
  int i;

  for (i = 0; i < 16; i++)
    ctr_preload[i] = 0x00;
  i = 0;

  ctr_preload[0] = 0x01; /* flag */
  if (qc_exists && a4_exists)
    ctr_preload[1] = mpdu[30] & 0x0f; /* QoC_Control */
  if (qc_exists && !a4_exists)
    ctr_preload[1] = mpdu[24] & 0x0f;

  for (i = 2; i < 8; i++)
    ctr_preload[i] = mpdu[i + 8]; /* ctr_preload[2:7] = A2[0:5] = mpdu[10:15] */
  for (i = 8; i < 14; i++)
    ctr_preload[i] = pn_vector[13 - i];       /* ctr_preload[8:13] = PN[5:0] */
  ctr_preload[14] = (unsigned char)(c / 256); /* Ctr */
  ctr_preload[15] = (unsigned char)(c % 256);
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
    // static_instructions_O0 : 81
    // dynamic_instructions_O0 : 362
    // -------------------------------
    // static_instructions_O1 : 40
    // dynamic_instructions_O1 : 90
    // -------------------------------
    // static_instructions_O2 : 50
    // dynamic_instructions_O2 : 50
    // -------------------------------
    // static_instructions_O3 : 50
    // dynamic_instructions_O3 : 50
    // -------------------------------
    // static_instructions_Ofast : 50
    // dynamic_instructions_Ofast : 50
    // -------------------------------
    // static_instructions_Os : 41
    // dynamic_instructions_Os : 91
    // -------------------------------
    // static_instructions_Oz : 45
    // dynamic_instructions_Oz : 191
    // -------------------------------

    int a4_exists = 255;

    int qc_exists = 255;

    int c = 255;

    int _len_ctr_preload0 = 65025;
    int *ctr_preload = (int *)malloc(_len_ctr_preload0 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_ctr_preload0; _i0++) {
      ctr_preload[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
    }

    int _len_mpdu0 = 65025;
    int *mpdu = (int *)malloc(_len_mpdu0 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_mpdu0; _i0++) {
      mpdu[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
    }

    int _len_pn_vector0 = 65025;
    int *pn_vector = (int *)malloc(_len_pn_vector0 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_pn_vector0; _i0++) {
      pn_vector[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
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

      construct_ctr_preload(ctr_preload, a4_exists, qc_exists, mpdu, pn_vector,
                            c);

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
      fprintf(f_csv, "construct_ctr_preload,%lu,%lu,%lu,%lu newline",
              count_cycles, count_instructions, count_cache_miss,
              count_cache_ref);

      fclose(f_csv);
    }
    free(ctr_preload);
    free(mpdu);
    free(pn_vector);

    break;
  }

  // big-arr-10x
  case 1: {
    // static_instructions_O0 : 81
    // dynamic_instructions_O0 : 362
    // -------------------------------
    // static_instructions_O1 : 40
    // dynamic_instructions_O1 : 90
    // -------------------------------
    // static_instructions_O2 : 50
    // dynamic_instructions_O2 : 50
    // -------------------------------
    // static_instructions_O3 : 50
    // dynamic_instructions_O3 : 50
    // -------------------------------
    // static_instructions_Ofast : 50
    // dynamic_instructions_Ofast : 50
    // -------------------------------
    // static_instructions_Os : 41
    // dynamic_instructions_Os : 91
    // -------------------------------
    // static_instructions_Oz : 45
    // dynamic_instructions_Oz : 191
    // -------------------------------

    int a4_exists = 10;

    int qc_exists = 10;

    int c = 10;

    int _len_ctr_preload0 = 100;
    int *ctr_preload = (int *)malloc(_len_ctr_preload0 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_ctr_preload0; _i0++) {
      ctr_preload[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
    }

    int _len_mpdu0 = 100;
    int *mpdu = (int *)malloc(_len_mpdu0 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_mpdu0; _i0++) {
      mpdu[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
    }

    int _len_pn_vector0 = 100;
    int *pn_vector = (int *)malloc(_len_pn_vector0 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_pn_vector0; _i0++) {
      pn_vector[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
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

      construct_ctr_preload(ctr_preload, a4_exists, qc_exists, mpdu, pn_vector,
                            c);

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
      fprintf(f_csv, "construct_ctr_preload,%lu,%lu,%lu,%lu newline",
              count_cycles, count_instructions, count_cache_miss,
              count_cache_ref);

      fclose(f_csv);
    }
    free(ctr_preload);
    free(mpdu);
    free(pn_vector);

    break;
  }

  default:
    usage();
    break;
  }

  return 0;
}
