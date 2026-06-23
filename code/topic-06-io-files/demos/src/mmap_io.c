/*
 * mmap_io.c — CST-315 · Topic 6 · demos · INSTRUCTOR ONLY
 * Purpose: Compare reading a file with read() vs mmap(). Often
 *          mmap is faster for large sequential reads because it
 *          skips a copy into a user buffer.
 * Build:   make mmap_io
 * Run:     ./mmap_io <path>
 * Notes:   Portable POSIX. Builds on macOS and Linux.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>

static double seconds(struct timespec a, struct timespec b) {
    return (b.tv_sec - a.tv_sec) + (b.tv_nsec - a.tv_nsec) / 1e9;
}

int main(int argc, char **argv) {
    if (argc < 2) { fprintf(stderr, "usage: %s <path>\n", argv[0]); return 1; }
    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) { perror(argv[1]); return 1; }
    struct stat st;
    fstat(fd, &st);

    struct timespec t0, t1, t2;

    /* read() into a heap buffer. */
    char *buf = malloc(st.st_size);
    clock_gettime(CLOCK_MONOTONIC, &t0);
    lseek(fd, 0, SEEK_SET);
    read(fd, buf, st.st_size);
    /* Touch every byte so the compiler doesn't optimize away. */
    volatile unsigned long sum1 = 0;
    for (long i = 0; i < st.st_size; i++) sum1 += (unsigned char)buf[i];
    clock_gettime(CLOCK_MONOTONIC, &t1);
    free(buf);

    /* mmap and scan. */
    char *m = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (m == MAP_FAILED) { perror("mmap"); close(fd); return 1; }
    volatile unsigned long sum2 = 0;
    for (long i = 0; i < st.st_size; i++) sum2 += (unsigned char)m[i];
    clock_gettime(CLOCK_MONOTONIC, &t2);
    munmap(m, st.st_size);
    close(fd);

    printf("file: %s   size: %lld bytes\n", argv[1], (long long)st.st_size);
    printf("read():   %.3f ms   (sum=%lu)\n", seconds(t0, t1) * 1000, sum1);
    printf("mmap():   %.3f ms   (sum=%lu)\n", seconds(t1, t2) * 1000, sum2);
    return 0;
}
