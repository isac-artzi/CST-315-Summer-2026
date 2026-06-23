/*
 * fd_table_demo.c — CST-315 · Topic 6 · demos · INSTRUCTOR ONLY
 * Purpose: Open the same file twice via two open() calls. Read one
 *          byte from each FD and observe that the offsets are
 *          independent — the two FDs share an inode but not a file
 *          position.
 * Build:   make fd_table_demo
 * Run:     ./fd_table_demo
 * Notes:   Portable POSIX.
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define PATH "/tmp/fdtable_demo.txt"

int main(void) {
    /* Set up a small file with known content. */
    int w = open(PATH, O_RDWR | O_CREAT | O_TRUNC, 0600);
    if (w < 0) { perror("open w"); return 1; }
    const char *body = "ABCDEFGHIJ";
    write(w, body, 10);
    close(w);

    int a = open(PATH, O_RDONLY);
    int b = open(PATH, O_RDONLY);
    if (a < 0 || b < 0) { perror("open"); return 1; }
    printf("opened twice — fd_a=%d, fd_b=%d\n", a, b);

    char ca, cb;
    read(a, &ca, 1); read(a, &ca, 1); read(a, &ca, 1);   /* a is at offset 3 */
    read(b, &cb, 1);                                     /* b is at offset 1 */
    printf("third byte via a: '%c' (expected 'C')\n", ca);
    printf("first byte via b: '%c' (expected 'A')\n", cb);
    printf("\nTwo FDs, one inode, two independent positions.\n");

    close(a); close(b);
    return 0;
}
