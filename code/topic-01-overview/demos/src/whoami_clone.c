/*
 * whoami_clone.c — CST-315 · Topic 1 · demos · INSTRUCTOR ONLY
 * Purpose: Reimplement `whoami` from scratch using getuid + getpwuid.
 *          Mirrors how a shell built-in would resolve the current user.
 * Build:   make whoami_clone
 * Run:     ./whoami_clone
 * Notes:   Portable POSIX. Builds on macOS and Linux.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>

int main(void) {
    /* getuid() returns the real user ID — never fails. */
    uid_t uid = getuid();

    /* Look it up in /etc/passwd (or its NSS equivalent). */
    struct passwd *pw = getpwuid(uid);
    if (!pw) { perror("getpwuid"); return 1; }

    printf("%s\n", pw->pw_name);
    return 0;
}
