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
typedef double REAL;

/* Variables and functions */

void matgen(REAL a[], int lda, int n, REAL b[], REAL *norma)

/* We would like to declare a[][lda], but c does not allow it.  In this
function, references to a[i][j] are written a[lda*i+j].  */

{
  int init, i, j;

  init = 1325;
  *norma = 0.0;
  for (j = 0; j < n; j++) {
    for (i = 0; i < n; i++) {
      init = 3125 * init % 65536;
      a[lda * j + i] = (init - 32768.0) / 16384.0;
      *norma = (a[lda * j + i] > *norma) ? a[lda * j + i] : *norma;

      /* alternative for some compilers
      if (fabs(a[lda*j+i]) > *norma) *norma = fabs(a[lda*j+i]);
      */
    }
  }
  for (i = 0; i < n; i++) {
    b[i] = 0.0;
  }
  for (j = 0; j < n; j++) {
    for (i = 0; i < n; i++) {
      b[i] = b[i] + a[lda * j + i];
    }
  }
  return;
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
    // static_instructions_O0 : 110
    // dynamic_instructions_O0 : 3715430
    // -------------------------------
    // static_instructions_O1 : 81
    // dynamic_instructions_O1 : 1629237
    // -------------------------------
    // static_instructions_O2 : 164
    // dynamic_instructions_O2 : 1402814
    // -------------------------------
    // static_instructions_O3 : 164
    // dynamic_instructions_O3 : 1402814
    // -------------------------------
    // static_instructions_Ofast : 164
    // dynamic_instructions_Ofast : 1402814
    // -------------------------------
    // static_instructions_Os : 72
    // dynamic_instructions_Os : 1628212
    // -------------------------------
    // static_instructions_Oz : 62
    // dynamic_instructions_Oz : 1370910
    // -------------------------------

    int lda = 255;

    int n = 255;

    int _len_a0 = 65025;
    double *a = (double *)malloc(_len_a0 * sizeof(double));
    for (int _i0 = 0; _i0 < _len_a0; _i0++) {
      a[_i0] = ((-2.0 * (next_i() % 2)) + 1.0) *
               (-16777216.000000 +
                (float)next_f() / (((float)16777215.000000 /
                                    (16777215.000000 - -16777216.000000))));
    }

    int _len_b0 = 65025;
    double *b = (double *)malloc(_len_b0 * sizeof(double));
    for (int _i0 = 0; _i0 < _len_b0; _i0++) {
      b[_i0] = ((-2.0 * (next_i() % 2)) + 1.0) *
               (-16777216.000000 +
                (float)next_f() / (((float)16777215.000000 /
                                    (16777215.000000 - -16777216.000000))));
    }

    int _len_norma0 = 65025;
    double *norma = (double *)malloc(_len_norma0 * sizeof(double));
    for (int _i0 = 0; _i0 < _len_norma0; _i0++) {
      norma[_i0] = ((-2.0 * (next_i() % 2)) + 1.0) *
                   (-16777216.000000 +
                    (float)next_f() / (((float)16777215.000000 /
                                        (16777215.000000 - -16777216.000000))));
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

      matgen(a, lda, n, b, norma);

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
      fprintf(f_csv, "matgen,%lu,%lu,%lu,%lu newline", count_cycles,
              count_instructions, count_cache_miss, count_cache_ref);

      fclose(f_csv);
    }
    free(a);
    free(b);
    free(norma);

    break;
  }

  // big-arr-10x
  case 1: {
    // static_instructions_O0 : 110
    // dynamic_instructions_O0 : 6090
    // -------------------------------
    // static_instructions_O1 : 81
    // dynamic_instructions_O1 : 2682
    // -------------------------------
    // static_instructions_O2 : 149
    // dynamic_instructions_O2 : 2669
    // -------------------------------
    // static_instructions_O3 : 149
    // dynamic_instructions_O3 : 2669
    // -------------------------------
    // static_instructions_Ofast : 149
    // dynamic_instructions_Ofast : 2669
    // -------------------------------
    // static_instructions_Os : 72
    // dynamic_instructions_Os : 2637
    // -------------------------------
    // static_instructions_Oz : 62
    // dynamic_instructions_Oz : 2340
    // -------------------------------

    int lda = 10;

    int n = 10;

    int _len_a0 = 100;
    double *a = (double *)malloc(_len_a0 * sizeof(double));
    for (int _i0 = 0; _i0 < _len_a0; _i0++) {
      a[_i0] = ((-2.0 * (next_i() % 2)) + 1.0) *
               (-16777216.000000 +
                (float)next_f() / (((float)16777215.000000 /
                                    (16777215.000000 - -16777216.000000))));
    }

    int _len_b0 = 100;
    double *b = (double *)malloc(_len_b0 * sizeof(double));
    for (int _i0 = 0; _i0 < _len_b0; _i0++) {
      b[_i0] = ((-2.0 * (next_i() % 2)) + 1.0) *
               (-16777216.000000 +
                (float)next_f() / (((float)16777215.000000 /
                                    (16777215.000000 - -16777216.000000))));
    }

    int _len_norma0 = 100;
    double *norma = (double *)malloc(_len_norma0 * sizeof(double));
    for (int _i0 = 0; _i0 < _len_norma0; _i0++) {
      norma[_i0] = ((-2.0 * (next_i() % 2)) + 1.0) *
                   (-16777216.000000 +
                    (float)next_f() / (((float)16777215.000000 /
                                        (16777215.000000 - -16777216.000000))));
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

      matgen(a, lda, n, b, norma);

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
      fprintf(f_csv, "matgen,%lu,%lu,%lu,%lu newline", count_cycles,
              count_instructions, count_cache_miss, count_cache_ref);

      fclose(f_csv);
    }
    free(a);
    free(b);
    free(norma);

    break;
  }

  // empty
  case 2: {
    // static_instructions_O0 : 25
    // dynamic_instructions_O0 : 25
    // -------------------------------
    // static_instructions_O1 : 28
    // dynamic_instructions_O1 : 28
    // -------------------------------
    // static_instructions_O2 : 19
    // dynamic_instructions_O2 : 19
    // -------------------------------
    // static_instructions_O3 : 19
    // dynamic_instructions_O3 : 19
    // -------------------------------
    // static_instructions_Ofast : 19
    // dynamic_instructions_Ofast : 19
    // -------------------------------
    // static_instructions_Os : 19
    // dynamic_instructions_Os : 19
    // -------------------------------
    // static_instructions_Oz : 30
    // dynamic_instructions_Oz : 30
    // -------------------------------

    int lda = ((-2 * (next_i() % 2)) + 1) * next_i();

    int n = ((-2 * (next_i() % 2)) + 1) * next_i();

    int _len_a0 = 1;
    double *a = (double *)malloc(_len_a0 * sizeof(double));
    for (int _i0 = 0; _i0 < _len_a0; _i0++) {
      a[_i0] = ((-2.0 * (next_i() % 2)) + 1.0) *
               (-16777216.000000 +
                (float)next_f() / (((float)16777215.000000 /
                                    (16777215.000000 - -16777216.000000))));
    }

    int _len_b0 = 1;
    double *b = (double *)malloc(_len_b0 * sizeof(double));
    for (int _i0 = 0; _i0 < _len_b0; _i0++) {
      b[_i0] = ((-2.0 * (next_i() % 2)) + 1.0) *
               (-16777216.000000 +
                (float)next_f() / (((float)16777215.000000 /
                                    (16777215.000000 - -16777216.000000))));
    }

    int _len_norma0 = 1;
    double *norma = (double *)malloc(_len_norma0 * sizeof(double));
    for (int _i0 = 0; _i0 < _len_norma0; _i0++) {
      norma[_i0] = ((-2.0 * (next_i() % 2)) + 1.0) *
                   (-16777216.000000 +
                    (float)next_f() / (((float)16777215.000000 /
                                        (16777215.000000 - -16777216.000000))));
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

      matgen(a, lda, n, b, norma);

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
      fprintf(f_csv, "matgen,%lu,%lu,%lu,%lu newline", count_cycles,
              count_instructions, count_cache_miss, count_cache_ref);

      fclose(f_csv);
    }
    free(a);
    free(b);
    free(norma);

    break;
  }

  default:
    usage();
    break;
  }

  return 0;
}
