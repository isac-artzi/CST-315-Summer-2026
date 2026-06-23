/*
 * hello.c — CST-315 · Topic 1 · starter
 * Purpose: Smoke-test the Linux/UNIX dev environment and demonstrate
 *          basic environment-variable access.
 * Build:   make
 * Run:     ./hello
 *
 * STUDENT TODO: complete this file per the README and Project 1 spec.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    /* ===========================================================
     * TODO Topic 1: greet a name and print the course tagline.
     *
     * Pick a name to greet, in priority order:
     *   1. argv[1] if argc > 1 and argv[1] is non-empty.
     *   2. The environment variable USER_OVERRIDE if set & non-empty.
     *   3. The environment variable USER if set & non-empty.
     *   4. The literal string "world".
     *
     * Then print TWO lines:
     *   Hello, <name>!
     *   This is CST-315: Operating Systems.
     *
     * Hints:
     *   const char *value = getenv("VAR");  // NULL if unset
     *   value && value[0]                   // true if non-empty
     *   printf("Hello, %s!\n", name);
     *
     * Replace the placeholder line below with your implementation,
     * then run `make test` to check.
     * ===========================================================
     */

    /* TODO: replace this stub with your implementation */
    (void)argc; (void)argv;
    printf("TODO: complete me\n");

    return 0;
}
