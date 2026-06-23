/*
 * lopeShell.c — CST-315 · Topic 2 · starter
 * Purpose: Minimal command-line interpreter. One command per line.
 *          Forks a child, execs the command, waits for it to exit.
 *          Supports interactive and batch mode plus an "exit" builtin.
 * Build:   make
 * Run:     ./lopeShell                  (interactive)
 *          ./lopeShell my_batch.txt     (batch)
 *
 * STUDENT TODO: complete the two marked blocks below.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 1024
#define MAX_ARGS 64
#define PROMPT   "lopeShell> "

/* Tokenize `line` in place on whitespace; fill argv up to MAX_ARGS-1.
 * Return argc. argv[argc] must be NULL. */
static int tokenize(char *line, char *argv[MAX_ARGS]) {
    /* ===========================================================
     * TODO Topic 2 (1 of 2): tokenize `line` into argv.
     *
     * Split `line` on whitespace (spaces and tabs). Fill argv[0..]
     * with pointers into `line`. Stop at MAX_ARGS - 1 tokens.
     * Set argv[argc] = NULL so execvp() knows where argv ends.
     * Return argc.
     *
     * Hint: strtok(line, " \t") returns the first token, then call
     *       strtok(NULL, " \t") to get the rest.
     * Hint: NULL-terminate argv even when there are 0 tokens.
     * =========================================================== */

    /* TODO: replace this stub */
    (void)line;
    argv[0] = NULL;
    return 0;
}

/* Fork-exec-wait a single tokenized command. */
static void run_command(char *argv[MAX_ARGS]) {
    /* ===========================================================
     * TODO Topic 2 (2 of 2): fork, child execs, parent waits.
     *
     * Steps:
     *   1. pid = fork()
     *   2. if pid < 0: perror, return.
     *   3. if pid == 0 (child): execvp(argv[0], argv).
     *      If execvp returns, it failed — print an error and _exit(127).
     *   4. else (parent): waitpid(pid, &status, 0).
     *
     * Hint: fprintf(stderr, "lopeShell: %s: command not found\n", argv[0]);
     * Hint: Use _exit(127) NOT exit(127) in the child after exec failure.
     * =========================================================== */

    /* TODO: replace this stub */
    (void)argv;
    fprintf(stderr, "lopeShell: TODO: implement run_command\n");
}

int main(int argc, char **argv) {
    FILE *input = stdin;
    int interactive = 1;

    /* Optional batch mode: first argument is a file of commands. */
    if (argc > 1) {
        input = fopen(argv[1], "r");
        if (!input) { perror(argv[1]); return 1; }
        interactive = 0;
    }

    char line[MAX_LINE];
    while (1) {
        if (interactive) {
            fputs(PROMPT, stdout);
            fflush(stdout);
        }
        if (fgets(line, sizeof(line), input) == NULL) break;
        line[strcspn(line, "\n")] = '\0';
        if (line[0] == '\0') continue;
        if (strcmp(line, "exit") == 0) break;

        char *cmd_argv[MAX_ARGS];
        int cmd_argc = tokenize(line, cmd_argv);
        if (cmd_argc == 0) continue;

        run_command(cmd_argv);
    }

    if (!interactive) fclose(input);
    return 0;
}
