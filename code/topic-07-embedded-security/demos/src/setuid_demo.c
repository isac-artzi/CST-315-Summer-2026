/*
 * setuid_demo.c — CST-315 · Topic 7 · demos · INSTRUCTOR ONLY
 * Purpose: Print real and effective user IDs. If the binary has
 *          the setuid bit set (chmod u+s + owned by root), ruid
 *          will differ from euid when run by an unprivileged user.
 *          Demonstrates the principle behind sudo, mount, ping.
 * Build:   make setuid_demo
 * Run:     ./setuid_demo
 *          # to demo privilege elevation:
 *          # sudo chown root setuid_demo && sudo chmod u+s setuid_demo
 *          # ./setuid_demo
 * Notes:   Portable POSIX. macOS and Linux both honor setuid bit.
 */

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
    uid_t r = getuid();
    uid_t e = geteuid();
    printf("real uid:      %u\n", (unsigned)r);
    printf("effective uid: %u\n", (unsigned)e);
    if (r == e) printf("(no privilege escalation — setuid bit not set or owner = caller)\n");
    else        printf("(setuid escalation active: running with privileges of euid %u)\n", (unsigned)e);
    return 0;
}
