/*
 * hello.c — CST-315 · Topic 1 · milestone
 * Purpose: Smoke-test the Linux/UNIX dev environment and demonstrate
 *          basic environment-variable access. First deliverable of
 *          Project 1 (Familiarity with UNIX/Linux).
 * Build:   make
 * Run:     ./hello
 *          ./hello Isac
 *          USER_OVERRIDE=alice ./hello
 * Notes:   Pure POSIX. Builds identically on macOS and Linux.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    /*
     * Pick a name to greet. Priority order:
     *   1. argv[1] if present and non-empty
     *   2. $USER_OVERRIDE (used by the smoke test)
     *   3. $USER (set by the login shell)
     *   4. literal "world"
     */
    const char *name = "world";

    if (argc > 1 && argv[1][0] != '\0') {
        name = argv[1];
    } else {
        const char *override = getenv("USER_OVERRIDE");
        const char *user     = getenv("USER");
        if (override && override[0]) {
            name = override;
        } else if (user && user[0]) {
            name = user;
        }
    }

    printf("Hello, %s!\n", name);
    printf("This is CST-315: Operating Systems.\n");
    return 0;
}
