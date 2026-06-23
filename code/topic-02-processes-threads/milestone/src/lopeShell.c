/*
 * lopeShell.c — CST-315 · Topic 2 · milestone
 * Purpose: Minimal command-line interpreter. One command per line.
 *          Forks a child, execs the command, waits for it to exit.
 *          Supports interactive and batch mode plus an "exit" builtin.
 * Build:   make
 * Run:     ./lopeShell                  (interactive)
 *          ./lopeShell my_batch.txt     (batch)
 * Notes:   Portable POSIX. Builds on macOS and Linux.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 1024
#define MAX_ARGS 64
#define PROMPT   "lopeShell> "

/* Tokenize `line` in place on whitespace, fill argv up to MAX_ARGS-1.
 * Returns argc. argv[argc] is set to NULL. */
static int tokenize(char *line, char *argv[MAX_ARGS]) {
    int argc = 0;
    char *tok = strtok(line, " \t");
    while (tok != NULL && argc < MAX_ARGS - 1) {
        argv[argc++] = tok;
        tok = strtok(NULL, " \t");
    }
    argv[argc] = NULL;
    return argc;
}

/* Fork-exec-wait a single tokenized command. */
static void run_command(char *argv[MAX_ARGS]) {
    pid_t pid = fork();
    if (pid < 0) { perror("lopeShell: fork"); return; }
    if (pid == 0) {
        execvp(argv[0], argv);
        /* Only reached on exec failure. */
        fprintf(stderr, "lopeShell: %s: command not found\n", argv[0]);
        _exit(127);
    }
    /* Parent: block until the child terminates. */
    int status;
    waitpid(pid, &status, 0);
    (void)status;
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

        /* Strip trailing newline */
        line[strcspn(line, "\n")] = '\0';

        /* Skip blank lines */
        if (line[0] == '\0') continue;

        /* "exit" builtin */
        if (strcmp(line, "exit") == 0) break;

        /* Tokenize and dispatch */
        char *cmd_argv[MAX_ARGS];
        int cmd_argc = tokenize(line, cmd_argv);
        if (cmd_argc == 0) continue;

        run_command(cmd_argv);
    }

    if (!interactive) fclose(input);
    return 0;
}
