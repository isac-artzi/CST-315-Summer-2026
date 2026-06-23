/*
 * virus.c — CST-315 · Topic 7 · milestone
 * Purpose: Demonstration command-injection virus per Assignment 4.
 *          The user types a command; the virus rewrites argv before
 *          calling execvp(). Uses NO system() — argv-construction
 *          is itself the attack surface.
 *
 *          For safety in this demo, the rewritten command is harmless:
 *          we replace 'rm <file>' with 'echo VIRUS would-have-rm <file>'.
 *          Real Assignment 4 work substitutes a different payload.
 *
 * Build:   make virus
 * Run:     ./virus rm myfile
 *          ./virus ls -l           (passed through unchanged)
 * Notes:   POSIX. Builds on macOS and Linux.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <cmd> [args...]\n", argv[0]);
        return 1;
    }
    /* Build a new argv. If user typed 'rm X', rewrite to
     * 'echo VIRUS would-have-rm X' — no actual deletion. */
    char *new_argv[64];
    int new_argc = 0;

    if (strcmp(argv[1], "rm") == 0) {
        new_argv[new_argc++] = "echo";
        new_argv[new_argc++] = "VIRUS";
        new_argv[new_argc++] = "would-have-rm";
        for (int i = 2; i < argc && new_argc < 60; i++)
            new_argv[new_argc++] = argv[i];
        fprintf(stderr, "[virus] argv rewritten: rm → echo VIRUS would-have-rm ...\n");
    } else {
        /* Pass through unchanged. */
        for (int i = 1; i < argc && new_argc < 60; i++)
            new_argv[new_argc++] = argv[i];
        fprintf(stderr, "[virus] argv unchanged\n");
    }
    new_argv[new_argc] = NULL;

    execvp(new_argv[0], new_argv);
    perror("execvp");
    return 1;
}
