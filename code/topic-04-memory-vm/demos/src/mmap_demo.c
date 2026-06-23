/*
 * mmap_demo.c — CST-315 · Topic 4 · demos · INSTRUCTOR ONLY
 * Purpose: Memory-map a file, modify it through memory, see the
 *          change land on disk. The OS page cache and the virtual
 *          memory subsystem are the same machinery.
 * Build:   make mmap_demo
 * Run:     ./mmap_demo
 * Notes:   Portable POSIX. Builds on macOS and Linux.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define PATH "/tmp/mmap_demo.txt"

int main(void) {
    /* Create / truncate the file with some initial content. */
    int fd = open(PATH, O_RDWR | O_CREAT | O_TRUNC, 0600);
    if (fd < 0) { perror("open"); return 1; }
    const char *initial = "Hello, mmap!\n";
    size_t len = strlen(initial);
    write(fd, initial, len);

    /* Map it into our address space. */
    char *p = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (p == MAP_FAILED) { perror("mmap"); close(fd); return 1; }

    printf("Before edit, file says: %.*s", (int)len, p);
    /* Edit through the mapping — no write() syscall. */
    memcpy(p, "Goodbye!", 8);
    msync(p, len, MS_SYNC);
    printf("After edit  through mmap: %.*s", (int)len, p);

    munmap(p, len);
    close(fd);
    printf("Now: cat %s   →   the change persisted to disk.\n", PATH);
    return 0;
}
