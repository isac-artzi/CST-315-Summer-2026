/*
 * lopeShell.c — CST-315 · Topic 3 · starter (v2)
 * Purpose: Extend the Topic 2 shell with concurrent multi-command
 *          dispatch (commands separated by ';'), a `cd` builtin, and
 *          a CTRL-X exit token.
 *
 * STUDENT TODO: complete the three marked blocks below.
 *
 * The Topic 2 mechanics are kept here as reference. You may replace
 * them with your own version from last week if you prefer.
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

/* SIGINT handler — print a hint instead of dying. */
static volatile sig_atomic_t got_sigint = 0;
static void on_sigint(int sig) { (void)sig; got_sigint = 1; }

/* === Reference from Topic 2. You may replace with your own version. === */
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

/* Split a single shell line on ';' into up to MAX_CMDS substrings.
 * Returns the number of commands found. */
static int split_commands(char *line, char *cmds[MAX_CMDS]) {
    /* ===========================================================
     * TODO Topic 3 (1 of 3): split `line` on ';'.
     *
     * Walk `line` and treat ';' as a separator. Fill cmds[0..n-1]
     * with pointers into `line`, stripping leading whitespace from
     * each so tokenize() works cleanly on the result. Return n.
     *
     * Hint: strtok(line, ";") then strtok(NULL, ";").
     * Hint: while (*p == ' ' || *p == '\t') p++;  // skip leading WS
     * =========================================================== */

    /* TODO: replace this stub. For now we treat the whole line as one. */
    cmds[0] = line;
    return 1;
}

/* Handle `cd` in the parent. Return 1 if handled, 0 if not a builtin. */
static int try_builtin(int argc, char *argv[MAX_ARGS]) {
    if (argc == 0) return 1;

    /* ===========================================================
     * TODO Topic 3 (2 of 3): implement `cd`.
     *
     * If argv[0] is "cd":
     *   - If argv[1] is provided, chdir(argv[1]).
     *   - Otherwise, chdir(getenv("HOME")). If HOME is not set,
     *     fall back to "/".
     *   - On chdir failure, perror("cd").
     *   - Return 1 (handled).
     *
     * Otherwise, return 0 (not a builtin).
     * =========================================================== */

    /* TODO: replace this stub */
    (void)argv;
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

        /* CTRL-X token (ASCII 0x18) → clean exit. */
        if (strchr(line, 0x18) != NULL) break;

        if (line[0] == '\0') continue;
        if (strcmp(line, "exit") == 0) break;

        char *cmd_strs[MAX_CMDS];
        int ncmds = split_commands(line, cmd_strs);

        /* ===========================================================
         * TODO Topic 3 (3 of 3): multi-command concurrent dispatch.
         *
         * For each cmd_strs[i]:
         *   tokenize it into a local argv.
         *   if try_builtin handles it, continue (no fork).
         *   else fork; child execvp's; on exec failure print and _exit(127).
         *   store the child PID in an array kids[].
         *
         * After ALL forks, reap every child with waitpid(kids[i], ...).
         *
         * Do NOT fork-wait-fork-wait. Fork-all, then wait-all.
         * =========================================================== */

        /* TODO: replace this stub */
        (void)ncmds;
        fprintf(stderr, "lopeShell: TODO: implement multi-command dispatch\n");
    }

    if (!interactive) fclose(input);
    return 0;
}
