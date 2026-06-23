/*
 * aslr_demo.c — CST-315 · Topic 7 · demos · INSTRUCTOR ONLY
 * Purpose: Print the address of a stack variable, a heap allocation,
 *          and a function. Run twice — addresses change. That's ASLR.
 * Build:   make aslr_demo
 * Run:     ./aslr_demo
 *          ./aslr_demo
 * Notes:   Portable POSIX. On macOS and modern Linux, ASLR is on by
 *          default.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(void) {
    int stack_var = 42;
    int *heap_var = malloc(sizeof(int));
    /* Use a function-pointer variable so the cast is well-defined. */
    int (*fp)(void) = main;
    printf("address of main():    %p\n", (void *)(uintptr_t)fp);
    printf("address of stack var: %p\n", (void *)&stack_var);
    printf("address of heap var:  %p\n", (void *)heap_var);
    free(heap_var);
    return 0;
}
