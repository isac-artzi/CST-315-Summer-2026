/*
 * lopeShell.c — CST-315 · Topic 3 · milestone (v2)
 * Purpose: Command-line interpreter that handles semicolon-separated
 *          multi-command lines concurrently. Adds `cd` builtin and a
 *          CTRL-X (ASCII 0x18) exit token. SIGINT is caught and turned
 *          into a friendly prompt instead of killing the shell.
 * Build:   make
 * Run:     ./lopeShell                  (interactive)
 *          ./lopeShell my_batch.txt     (batch)
 * Notes:   Portable POSIX. Builds on macOS and Linux.
 *          Cumulative on Topic 2; new code marked with "v2:".
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE  1024
#define MAX_ARGS  64
#define MAX_CMDS  16
#define PROMPT    "lopeShell> "

/* v2: SIGINT handler — print a hint instead of dying. */
static volatile sig_atomic_t got_sigint = 0;
static void on_sigint(int sig) { (void)sig; got_sigint = 1; }

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

/* v2: split a line on ';' into up to MAX_CMDS substrings (in place).
 * Returns the number of commands. */
static int split_commands(char *line, char *cmds[MAX_CMDS]) {
    int n = 0;
    char *p = strtok(line, ";");
    while (p != NULL && n < MAX_CMDS) {
        /* Skip leading whitespace for cleaner tokenization later. */
        while (*p == ' ' || *p == '\t') p++;
        cmds[n++] = p;
        p = strtok(NULL, ";");
    }
    return n;
}

/* v2: builtins return 1 if handled, 0 if not a builtin. */
static int try_builtin(int argc, char *argv[MAX_ARGS]) {
    if (argc == 0) return 1;
    if (strcmp(argv[0], "cd") == 0) {
        const char *dest = (argc > 1) ? argv[1] : getenv("HOME");
        if (!dest) dest = "/";
        if (chdir(dest) != 0) perror("cd");
        return 1;
    }
    return 0;
}

int main(int argc, char **argv) {
    FILE *input = stdin;
    int interactive = 1;
    if (argc > 1) {
        input = fopen(argv[1], "r");
        if (!input) { perror(argv[1]); return 1; }
        interactive = 0;
    }

    /* v2: install SIGINT handler so CTRL-C doesn't kill the shell. */
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = on_sigint;
    sigaction(SIGINT, &sa, NULL);

    char line[MAX_LINE];
    while (1) {
        if (interactive) {
            if (got_sigint) {
                fputs("\n(use 'exit' or CTRL-X to leave)\n", stdout);
                got_sigint = 0;
            }
            fputs(PROMPT, stdout);
            fflush(stdout);
        }
        if (fgets(line, sizeof(line), input) == NULL) break;
        line[strcspn(line, "\n")] = '\0';

        /* v2: CTRL-X token (ASCII 0x18) → clean exit. */
        if (strchr(line, 0x18) != NULL) break;

        if (line[0] == '\0') continue;
        if (strcmp(line, "exit") == 0) break;

        /* v2: split on ';' for concurrent dispatch. */
        char *cmd_strs[MAX_CMDS];
        int ncmds = split_commands(line, cmd_strs);

        /* Fork ALL commands first, then wait for ALL of them.
         * This gives concurrent execution within a single line. */
        pid_t kids[MAX_CMDS];
        int nkids = 0;
        for (int i = 0; i < ncmds; i++) {
            char *cargv[MAX_ARGS];
            int cargc = tokenize(cmd_strs[i], cargv);
            if (cargc == 0) continue;

            /* v2: builtins run in the parent (no fork). */
            if (try_builtin(cargc, cargv)) continue;

            pid_t pid = fork();
            if (pid < 0) { perror("lopeShell: fork"); continue; }
            if (pid == 0) {
                execvp(cargv[0], cargv);
                fprintf(stderr, "lopeShell: %s: command not found\n", cargv[0]);
                _exit(127);
            }
            kids[nkids++] = pid;
        }

        /* Reap all children. */
        for (int i = 0; i < nkids; i++) {
            int status;
            waitpid(kids[i], &status, 0);
            (void)status;
        }
    }

    if (!interactive) fclose(input);
    return 0;
}
