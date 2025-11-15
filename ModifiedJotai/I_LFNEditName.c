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
typedef int /*<<< orphan*/ WORD;
typedef int TCHAR;
typedef int *LPTSTR;
typedef scalar_t__ INT;

/* Variables and functions */
#define CHAR_DOT 130
int CHAR_NULL;
#define CHAR_QUESTION 129
#define CHAR_STAR 128
int DOT;
int /*<<< orphan*/ ERROR_INVALID_PARAMETER;
int /*<<< orphan*/ NO_ERROR;

WORD I_LFNEditName(LPTSTR lpSrc, LPTSTR lpEd, LPTSTR lpRes, INT iResBufSize) {
  INT ResLen = 0; // Length of result

  // This is turned off until we agree
  // that cmd operates in the same way

#ifdef USELASTDOT
  LPTSTR lpChar;

  //
  // We have a special case hack for the dot, since when we do a
  // rename from foo.bar.baz to *.txt, we want to use the last dot, not
  // the first one (desired result = foo.bar.txt, not foo.txt)
  //
  // We find the dot (GetExtension rets first char after last dot if there
  // is a dot), then if the delimiter for the '*' is a CHAR_DOT, we continue
  // copying until we get to the last dot instead of finding the first one.
  //

  lpChar = GetExtension(lpSrc);
  if (*lpChar) {
    lpChar--;
  } else {
    lpChar = NULL;
  }
#endif

  while (*lpEd) {

    if (ResLen < iResBufSize) {

      switch (*lpEd) {

      case CHAR_STAR: {
        TCHAR delimit = *(lpEd + 1);

#ifdef USELASTDOT

        //
        // For all other delimiters, we use the first
        // occurrence (e.g., *f.txt).
        //
        if (CHAR_DOT != delimit)
          lpChar = NULL;

        while ((ResLen < iResBufSize) && (*lpSrc != CHAR_NULL) &&
               (*lpSrc != delimit || (lpChar && lpChar != lpSrc))) {
#else
        while ((ResLen < iResBufSize) && (*lpSrc != CHAR_NULL) &&
               (*lpSrc != delimit)) {
#endif

          *(lpRes++) = *(lpSrc++);
          ResLen++;
        }
      } break;

      case CHAR_QUESTION:
        if ((*lpSrc != DOT) && (*lpSrc != CHAR_NULL)) {

          if (ResLen < iResBufSize) {

            *(lpRes++) = *(lpSrc++);
            ResLen++;
          } else
            return ERROR_INVALID_PARAMETER;
        }
        break;

      case CHAR_DOT:
        while ((*lpSrc != DOT) && (*lpSrc != CHAR_NULL))
          lpSrc++;

        *(lpRes++) = DOT; // from EditMask, even if src doesn't
                          // have one, so always put one.
        ResLen++;
        if (*lpSrc) // point one past CHAR_DOT
          lpSrc++;
        break;

      default:
        if ((*lpSrc != DOT) && (*lpSrc != CHAR_NULL)) {

          lpSrc++;
        }

        if (ResLen < iResBufSize) {

          *(lpRes++) = *lpEd;
          ResLen++;
        } else
          return ERROR_INVALID_PARAMETER;
        break;
      }
      lpEd++;

    } else {

      return ERROR_INVALID_PARAMETER;
    }
  }

  if ((ResLen) < iResBufSize) {
    *lpRes = CHAR_NULL;
    return NO_ERROR;
  } else
    return ERROR_INVALID_PARAMETER;
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
    long iResBufSize = 255;

    int _len_lpSrc0 = 65025;
    int *lpSrc = (int *)malloc(_len_lpSrc0 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_lpSrc0; _i0++) {
      lpSrc[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
    }

    int _len_lpEd0 = 65025;
    int *lpEd = (int *)malloc(_len_lpEd0 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_lpEd0; _i0++) {
      lpEd[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
    }

    int _len_lpRes0 = 65025;
    int *lpRes = (int *)malloc(_len_lpRes0 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_lpRes0; _i0++) {
      lpRes[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
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

      __typeof__(I_LFNEditName(lpSrc, lpEd, lpRes, iResBufSize)) __perf_ret =
          I_LFNEditName(lpSrc, lpEd, lpRes, iResBufSize);

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
      fprintf(f_csv, "I_LFNEditName,%lu,%lu,%lu,%lu newline", count_cycles,
              count_instructions, count_cache_miss, count_cache_ref);

      fclose(f_csv);

      __perf_ret;
    });
    printf("%d\n", benchRet);
    free(lpSrc);
    free(lpEd);
    free(lpRes);

    break;
  }
  // big-arr-10x
  case 1: {
    long iResBufSize = 10;

    int _len_lpSrc0 = 100;
    int *lpSrc = (int *)malloc(_len_lpSrc0 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_lpSrc0; _i0++) {
      lpSrc[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
    }

    int _len_lpEd0 = 100;
    int *lpEd = (int *)malloc(_len_lpEd0 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_lpEd0; _i0++) {
      lpEd[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
    }

    int _len_lpRes0 = 100;
    int *lpRes = (int *)malloc(_len_lpRes0 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_lpRes0; _i0++) {
      lpRes[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
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

      __typeof__(I_LFNEditName(lpSrc, lpEd, lpRes, iResBufSize)) __perf_ret =
          I_LFNEditName(lpSrc, lpEd, lpRes, iResBufSize);

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
      fprintf(f_csv, "I_LFNEditName,%lu,%lu,%lu,%lu newline", count_cycles,
              count_instructions, count_cache_miss, count_cache_ref);

      fclose(f_csv);

      __perf_ret;
    });
    printf("%d\n", benchRet);
    free(lpSrc);
    free(lpEd);
    free(lpRes);

    break;
  }
  // empty
  case 2: {
    long iResBufSize = ((-2 * (next_i() % 2)) + 1) * next_i();

    int _len_lpSrc0 = 1;
    int *lpSrc = (int *)malloc(_len_lpSrc0 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_lpSrc0; _i0++) {
      lpSrc[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
    }

    int _len_lpEd0 = 1;
    int *lpEd = (int *)malloc(_len_lpEd0 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_lpEd0; _i0++) {
      lpEd[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
    }

    int _len_lpRes0 = 1;
    int *lpRes = (int *)malloc(_len_lpRes0 * sizeof(int));
    for (int _i0 = 0; _i0 < _len_lpRes0; _i0++) {
      lpRes[_i0] = ((-2 * (next_i() % 2)) + 1) * next_i();
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

      __typeof__(I_LFNEditName(lpSrc, lpEd, lpRes, iResBufSize)) __perf_ret =
          I_LFNEditName(lpSrc, lpEd, lpRes, iResBufSize);

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
      fprintf(f_csv, "I_LFNEditName,%lu,%lu,%lu,%lu newline", count_cycles,
              count_instructions, count_cache_miss, count_cache_ref);

      fclose(f_csv);

      __perf_ret;
    });
    printf("%d\n", benchRet);
    free(lpSrc);
    free(lpEd);
    free(lpRes);

    break;
  }
  default:
    usage();
    break;
  }

  return 0;
}
