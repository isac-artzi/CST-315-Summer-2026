/*
 * lopeShell.c — CST-315 · Topic 7 · starter (v3)
 *
 * STUDENT TODO: implement safe_arg() at the marked block.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 1024
#define MAX_ARGS 64
#define MAX_CMDS 16
#define PROMPT   "lopeShell> "

static volatile sig_atomic_t got_sigint = 0;
static void on_sigint(int sig) { (void)sig; got_sigint = 1; }

static int tokenize(char *line, char *argv[MAX_ARGS]) {
    int argc = 0; char *tok = strtok(line, " \t");
    while (tok && argc < MAX_ARGS - 1) { argv[argc++] = tok; tok = strtok(NULL, " \t"); }
    argv[argc] = NULL; return argc;
}
static int split_commands(char *line, char *cmds[MAX_CMDS]) {
    int n = 0; char *p = strtok(line, ";");
    while (p && n < MAX_CMDS) {
        while (*p == ' ' || *p == '\t') p++;
        cmds[n++] = p; p = strtok(NULL, ";");
    }
    return n;
}
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

/* ===========================================================
 * TODO Topic 7: implement safe_arg.
 *
 * Return 1 if the string is safe — does NOT contain any of the
 * banned shell metacharacters: ; & | $ ` < > newline ' "
 * Otherwise return 0.
 *
 * Hint: strpbrk(s, banned) returns non-NULL if any character
 *       in `banned` appears in s; NULL if none does.
 * =========================================================== */
static int safe_arg(const char *s) {
    /* TODO: replace this stub */
    (void)s;
    return 1;
}

static int sanitize_argv(int argc, char *argv[MAX_ARGS]) {
    for (int i = 0; i < argc; i++) {
        if (!safe_arg(argv[i])) {
            fprintf(stderr, "lopeShell: rejected argv[%d] '%s' (contains metacharacter)\n", i, argv[i]);
            return 0;
        }
    }
    return 1;
}

int main(int argc, char **argv) {
    FILE *input = stdin; int interactive = 1;
    if (argc > 1) { input = fopen(argv[1], "r"); if (!input) { perror(argv[1]); return 1; } interactive = 0; }
    struct sigaction sa; memset(&sa, 0, sizeof(sa)); sa.sa_handler = on_sigint;
    sigaction(SIGINT, &sa, NULL);

    char line[MAX_LINE];
    while (1) {
        if (interactive) { if (got_sigint) { fputs("\n(use 'exit' or CTRL-X to leave)\n", stdout); got_sigint = 0; } fputs(PROMPT, stdout); fflush(stdout); }
        if (!fgets(line, sizeof(line), input)) break;
        line[strcspn(line, "\n")] = '\0';
        if (strchr(line, 0x18)) break;
        if (line[0] == '\0') continue;
        if (strcmp(line, "exit") == 0) break;

        char *cmd_strs[MAX_CMDS];
        int ncmds = split_commands(line, cmd_strs);
        pid_t kids[MAX_CMDS]; int nkids = 0;
        for (int i = 0; i < ncmds; i++) {
            char *cargv[MAX_ARGS]; int cargc = tokenize(cmd_strs[i], cargv);
            if (cargc == 0) continue;
            if (try_builtin(cargc, cargv)) continue;
            if (!sanitize_argv(cargc, cargv)) continue;
            pid_t pid = fork();
            if (pid < 0) { perror("fork"); continue; }
            if (pid == 0) { execvp(cargv[0], cargv); fprintf(stderr, "lopeShell: %s: command not found\n", cargv[0]); _exit(127); }
            kids[nkids++] = pid;
        }
        for (int i = 0; i < nkids; i++) waitpid(kids[i], NULL, 0);
    }
    if (!interactive) fclose(input);
    return 0;
}
