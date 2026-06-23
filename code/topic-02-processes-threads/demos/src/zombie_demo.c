/*
 * zombie_demo.c — CST-315 · Topic 2 · demos · INSTRUCTOR ONLY
 * Purpose: Make a zombie. Parent forks, child immediately exits,
 *          parent sleeps for 20s without calling wait(). During
 *          that window, `ps -ef | grep <child_pid>` shows <defunct>.
 * Build:   make zombie_demo
 * Run:     ./zombie_demo
 * Notes:   Portable POSIX. Builds on macOS and Linux. macOS shows
 *          state 'Z' in `ps -o pid,ppid,state,comm`.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
    pid_t pid = fork();
    if (pid < 0) { perror("fork"); return 1; }

    if (pid == 0) {
        /* Child: exit right away with status 42. */
        _exit(42);
    }

    /* Parent: deliberately do NOT call wait(). */
    printf("parent PID %d. child PID %d is now a zombie.\n", getpid(), pid);
    printf("In another terminal, try:  ps -o pid,ppid,state,comm -p %d\n", pid);
    printf("Sleeping 20 seconds, then we will finally reap.\n");
    sleep(20);

    int status;
    waitpid(pid, &status, 0);
    printf("Reaped. child exit status = %d\n", WEXITSTATUS(status));
    return 0;
}
