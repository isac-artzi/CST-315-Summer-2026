/*
 * signal_demo.c — CST-315 · Topic 2 · demos · INSTRUCTOR ONLY
 * Purpose: Install a SIGINT (CTRL-C) handler so the signal is
 *          intercepted instead of killing the process. Useful for
 *          previewing the CTRL-X exit shortcut in Topic 3.
 * Build:   make signal_demo
 * Run:     ./signal_demo
 *          press CTRL-C a few times, then type 'q' + Enter
 * Notes:   Portable POSIX. Builds on macOS and Linux.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

static volatile sig_atomic_t hits = 0;

static void on_sigint(int sig) {
    (void)sig;
    /* Async-signal-safe: only write() is fully safe here.
     * For demo purposes, a small printf is acceptable. */
    hits++;
    const char *msg = "\n[caught CTRL-C — type 'q' + Enter to quit]\n";
    write(STDERR_FILENO, msg, strlen(msg));
}

int main(void) {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = on_sigint;
    sigaction(SIGINT, &sa, NULL);

    printf("PID %d. Press CTRL-C as many times as you like, then 'q' + Enter.\n", getpid());

    char buf[64];
    while (fgets(buf, sizeof(buf), stdin)) {
        if (buf[0] == 'q') break;
        printf("you typed: %s", buf);
    }

    printf("done. CTRL-C was caught %d times.\n", (int)hits);
    return 0;
}
