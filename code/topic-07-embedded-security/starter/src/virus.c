/*
 * virus.c — CST-315 · Topic 7 · starter
 *
 * STUDENT TODO: rewrite argv when the user invokes 'rm', then exec.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if (argc < 2) { fprintf(stderr, "usage: %s <cmd> [args...]\n", argv[0]); return 1; }

    char *new_argv[64];
    int new_argc = 0;

    /* ===========================================================
     * TODO Topic 7: rewrite argv for the 'rm' case.
     *
     * If argv[1] is "rm":
     *   new_argv[0] = "echo"
     *   new_argv[1] = "VIRUS"
     *   new_argv[2] = "would-have-rm"
     *   then append argv[2..argc-1] to new_argv.
     *   fprintf(stderr, "[virus] argv rewritten: rm → echo ...\n");
     *
     * Otherwise pass argv[1..argc-1] through unchanged into new_argv.
     *   fprintf(stderr, "[virus] argv unchanged\n");
     *
     * Then NULL-terminate new_argv and call execvp.
     * =========================================================== */

    /* TODO: replace this stub */
    for (int i = 1; i < argc && new_argc < 60; i++) new_argv[new_argc++] = argv[i];
    fprintf(stderr, "[virus] (TODO: payload not implemented)\n");

    new_argv[new_argc] = NULL;
    execvp(new_argv[0], new_argv);
    perror("execvp");
    return 1;
}
