/*
 * stack_canary_demo.c — CST-315 · Topic 7 · demos · INSTRUCTOR ONLY
 * Purpose: Demonstrate stack canary detection. Overflow a small
 *          local buffer; the canary check at function return
 *          aborts with "*** stack smashing detected ***".
 * Build:   make stack_canary_demo            (compiles with -fstack-protector-strong)
 * Run:     ./stack_canary_demo               (will abort with canary detection)
 *          For a comparison build WITHOUT the protector:
 *          cc -o no_canary src/stack_canary_demo.c
 *          ./no_canary                       (corrupts stack silently)
 * Notes:   Portable POSIX. macOS clang and Linux gcc both support
 *          -fstack-protector-strong.
 */

#include <stdio.h>
#include <string.h>

static void victim(const char *input) {
    char buf[8];
    /* Intentional overflow if input > 7 bytes. The canary detects this. */
    strcpy(buf, input);
    printf("buf: %s\n", buf);
}

int main(void) {
    printf("calling victim with a 32-byte input into an 8-byte buffer...\n");
    victim("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
    printf("(unreachable — canary should have caught the overflow)\n");
    return 0;
}
