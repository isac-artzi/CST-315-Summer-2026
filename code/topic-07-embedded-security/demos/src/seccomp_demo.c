/*
 * seccomp_demo.c — CST-315 · Topic 7 · demos · INSTRUCTOR ONLY
 * Purpose: Install a seccomp filter that allows only write() and
 *          exit_group(). Then try to call open() — the kernel kills
 *          the process with SIGSYS. Modern sandboxing in 30 lines.
 * Build:   make seccomp_demo
 * Run:     ./seccomp_demo
 * Notes:   Linux only. On macOS prints a friendly skip message.
 */

#include <stdio.h>
#include <stdlib.h>

#ifdef __linux__

#include <linux/seccomp.h>
#include <linux/filter.h>
#include <linux/audit.h>
#include <sys/syscall.h>
#include <sys/prctl.h>
#include <unistd.h>
#include <fcntl.h>

#define ALLOW(name) \
    BPF_JUMP(BPF_JMP | BPF_JEQ | BPF_K, __NR_##name, 0, 1), \
    BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_ALLOW)

int main(void) {
    struct sock_filter filter[] = {
        BPF_STMT(BPF_LD | BPF_W | BPF_ABS, 0),   /* load syscall number */
        ALLOW(write),
        ALLOW(exit_group),
        ALLOW(exit),
        BPF_STMT(BPF_RET | BPF_K, SECCOMP_RET_TRAP),
    };
    struct sock_fprog prog = {
        .len = sizeof(filter) / sizeof(filter[0]),
        .filter = filter,
    };
    if (prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0) < 0) { perror("prctl"); return 1; }
    if (syscall(SYS_seccomp, SECCOMP_SET_MODE_FILTER, 0, &prog) < 0) {
        perror("seccomp"); return 1;
    }
    /* write() is allowed — this prints. */
    const char msg[] = "seccomp installed. about to try open(), which is NOT allowed...\n";
    write(1, msg, sizeof(msg) - 1);
    /* open() is NOT allowed — this kills the process with SIGSYS. */
    open("/etc/passwd", O_RDONLY);
    write(1, "(unreachable)\n", 14);
    return 0;
}

#else

int main(void) {
    printf("seccomp_demo: this demo is Linux-only.\n");
    printf("On macOS, see sandbox_init(3) / sandbox-exec for an analog.\n");
    return 0;
}

#endif
