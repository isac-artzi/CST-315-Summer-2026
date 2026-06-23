/*
 * bump_allocator.c — CST-315 · Topic 4 · demos · INSTRUCTOR ONLY
 * Purpose: Trivial bump allocator: one pointer, one cap, no free.
 *          The simplest possible memory allocator — useful to show
 *          that malloc's interface is universal but its policy varies.
 * Build:   make bump_allocator
 * Run:     ./bump_allocator
 * Notes:   Portable POSIX.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARENA_SIZE (1 << 20)   /* 1 MiB arena */

static char arena[ARENA_SIZE];
static size_t bump = 0;

static void *bump_alloc(size_t n) {
    /* Round up to 16-byte alignment. */
    size_t aligned = (n + 15) & ~(size_t)15;
    if (bump + aligned > ARENA_SIZE) return NULL;
    void *p = arena + bump;
    bump += aligned;
    return p;
}

int main(void) {
    char *a = bump_alloc(32);
    char *b = bump_alloc(128);
    char *c = bump_alloc(64);

    strcpy(a, "first");
    strcpy(b, "second, larger string");
    strcpy(c, "third");

    printf("a (32B):  '%s'   addr=%p\n", a, (void *)a);
    printf("b (128B): '%s'   addr=%p\n", b, (void *)b);
    printf("c (64B):  '%s'   addr=%p\n", c, (void *)c);
    printf("bump pointer: %zu / %d bytes used\n", bump, ARENA_SIZE);
    printf("\nNote: there is no free(). That's the cost of simplicity.\n");
    printf("Real allocators (jemalloc, glibc) free, coalesce, and reuse,\n");
    printf("which is why they are thousands of lines instead of ten.\n");
    return 0;
}
