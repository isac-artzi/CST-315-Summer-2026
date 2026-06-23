/*
 * pipe_demo.c — CST-315 · Topic 1 · demos · INSTRUCTOR ONLY
 * Purpose: Show how a shell wires `ls -l | wc -l` under the hood:
 *          create a pipe, fork two children, redirect their stdio,
 *          exec, and reap. Previews the plumbing the student's shell
 *          will eventually do.
 * Build:   make pipe_demo
 * Run:     ./pipe_demo
 * Notes:   Portable POSIX. Builds on macOS and Linux.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    int pipefd[2];                      /* [0] = read end, [1] = write end */
    if (pipe(pipefd) < 0) { perror("pipe"); return 1; }

    pid_t writer = fork();
    if (writer < 0) { perror("fork"); return 1; }
    if (writer == 0) {
        /* Child A: redirect stdout to pipe-write, exec "ls -l". */
        close(pipefd[0]);                       /* don't need read end */
        dup2(pipefd[1], STDOUT_FILENO);         /* stdout -> pipe write */
        close(pipefd[1]);                       /* close the duplicate */
        char *args[] = { "ls", "-l", NULL };
        execvp("ls", args);
        perror("execvp ls");
        _exit(1);
    }

    pid_t reader = fork();
    if (reader < 0) { perror("fork"); return 1; }
    if (reader == 0) {
        /* Child B: redirect stdin from pipe-read, exec "wc -l". */
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        char *args[] = { "wc", "-l", NULL };
        execvp("wc", args);
        perror("execvp wc");
        _exit(1);
    }

    /* Parent: close BOTH pipe ends, then wait for both children.
     * Closing both ends is critical — otherwise the reader's stdin
     * never sees EOF and wc -l hangs forever. */
    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(writer, NULL, 0);
    waitpid(reader, NULL, 0);
    return 0;
}
