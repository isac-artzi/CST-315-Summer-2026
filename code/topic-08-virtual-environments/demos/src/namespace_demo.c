/*
 * namespace_demo.c — CST-315 · Topic 8 · demos · INSTRUCTOR ONLY
 * Purpose: Use clone() to create a child in fresh UTS (hostname)
 *          and PID namespaces. The child sees itself as PID 1 and
 *          changes its hostname; the host is unaffected.
 *          This is the kernel mechanism Docker uses.
 * Build:   make namespace_demo
 * Run:     ./namespace_demo   (may need sudo on some systems for newuts/newpid)
 * Notes:   Linux only. On macOS prints a friendly skip message.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __linux__

/* clone(), CLONE_NEW* flags are Linux/glibc extensions.
 * Build with -D_GNU_SOURCE; the Makefile does this. */
#include <sched.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define STACK_SIZE (1024 * 1024)
static char child_stack[STACK_SIZE];

static int child_fn(void *arg) {
    (void)arg;
    printf("[child] getpid()    = %d (1 means we are in a fresh PID namespace)\n", getpid());
    sethostname("container-demo", strlen("container-demo"));
    char host[64] = {0};
    gethostname(host, sizeof(host));
    printf("[child] hostname    = %s\n", host);
    printf("[child] uname -n on the host will still show the original.\n");
    return 0;
}

int main(void) {
    pid_t pid = clone(child_fn, child_stack + STACK_SIZE,
                      CLONE_NEWUTS | CLONE_NEWPID | SIGCHLD,
                      NULL);
    if (pid < 0) { perror("clone"); fprintf(stderr, "(try with sudo)\n"); return 1; }
    waitpid(pid, NULL, 0);

    char host[64] = {0};
    gethostname(host, sizeof(host));
    printf("[parent] my hostname is still: %s\n", host);
    return 0;
}

#else

int main(void) {
    printf("namespace_demo: Linux only.\n");
    printf("Mac analog: app sandbox (sandbox_init / sandbox-exec).\n");
    return 0;
}

#endif
