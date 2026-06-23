/*
 * procfs_walker.c — CST-315 · Topic 1 · demos · INSTRUCTOR ONLY
 * Purpose: Read /proc/self/status and print the first 20 lines.
 *          The output IS a process control block — name, PID, PPID,
 *          state, UID/GID, memory size, threads, signal state, etc.
 *          Great for previewing the PCB concept before Topic 2.
 * Build:   make procfs_walker
 * Run:     ./procfs_walker
 * Notes:   Linux only. On macOS prints a friendly skip message.
 */

#include <stdio.h>
#include <stdlib.h>

#ifdef __linux__

int main(void) {
    FILE *f = fopen("/proc/self/status", "r");
    if (!f) { perror("/proc/self/status"); return 1; }
    char line[256];
    int n = 0;
    while (fgets(line, sizeof(line), f) && n < 20) {
        fputs(line, stdout);
        n++;
    }
    fclose(f);
    return 0;
}

#else  /* !__linux__ */

int main(void) {
    printf("procfs_walker: this demo is Linux-only.\n");
    printf("On macOS, try:  ps -o pid,ppid,state,user,comm -p $$\n");
    printf("Or:             vmmap $$  (memory layout)\n");
    return 0;
}

#endif
