/*
 * fault_timer.c — CST-315 · Topic 4 · demos · INSTRUCTOR ONLY
 * Purpose: Compare cold (first-touch, page fault) vs warm (cached)
 *          access cost. Allocate a multi-megabyte buffer, touch one
 *          byte per page on first pass (cold), then re-touch the
 *          same bytes (warm). The ratio is typically 10x–100x.
 * Build:   make fault_timer
 * Run:     ./fault_timer
 * Notes:   Portable POSIX. Builds on macOS and Linux.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define PAGE_SIZE 4096
#define PAGES     8192          /* 32 MiB */

static double seconds(struct timespec a, struct timespec b) {
    return (b.tv_sec - a.tv_sec) + (b.tv_nsec - a.tv_nsec) / 1e9;
}

int main(void) {
    size_t size = (size_t)PAGE_SIZE * PAGES;
    char *buf = calloc(1, size);          /* may be lazy-allocated */
    if (!buf) { perror("calloc"); return 1; }

    struct timespec t0, t1, t2;

    /* Cold: first touch of every page. */
    clock_gettime(CLOCK_MONOTONIC, &t0);
    volatile char sink = 0;
    for (int i = 0; i < PAGES; i++) sink ^= buf[i * PAGE_SIZE];
    clock_gettime(CLOCK_MONOTONIC, &t1);

    /* Warm: re-touch the same pages. */
    for (int i = 0; i < PAGES; i++) sink ^= buf[i * PAGE_SIZE];
    clock_gettime(CLOCK_MONOTONIC, &t2);
    (void)sink;

    double cold = seconds(t0, t1);
    double warm = seconds(t1, t2);
    printf("buffer:    %zu bytes (%d pages × %d B)\n", size, PAGES, PAGE_SIZE);
    printf("cold pass: %.3f ms   (%.2f ns / page)\n",
           cold * 1000, cold * 1e9 / PAGES);
    printf("warm pass: %.3f ms   (%.2f ns / page)\n",
           warm * 1000, warm * 1e9 / PAGES);
    printf("cold / warm: %.1fx\n", warm > 0 ? cold / warm : 0.0);
    free(buf);
    return 0;
}
