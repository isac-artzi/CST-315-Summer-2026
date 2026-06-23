/*
 * env_demo.c — CST-315 · Topic 1 · demos · INSTRUCTOR ONLY
 * Purpose: Show getenv, setenv, and how `environ` is structured.
 *          Useful for explaining how a shell exports variables to
 *          children it forks.
 * Build:   make env_demo
 * Run:     ./env_demo
 * Notes:   Portable POSIX. Builds on macOS and Linux.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* environ is a global pointer to a NULL-terminated array of
 * "KEY=VALUE" strings. It is set up by libc before main() runs. */
extern char **environ;

int main(void) {
    /* Read */
    const char *path = getenv("PATH");
    printf("PATH (first 80 chars): %.80s%s\n",
           path ? path : "(unset)",
           path && strlen(path) > 80 ? "..." : "");

    /* Write */
    setenv("CST315_DEMO", "hello-from-env-demo", 1 /* overwrite */);
    printf("CST315_DEMO = %s\n", getenv("CST315_DEMO"));

    /* Walk environ directly */
    printf("\nFirst 5 entries of environ:\n");
    for (int i = 0; environ[i] && i < 5; i++) {
        printf("  [%d] %.60s%s\n", i, environ[i],
               strlen(environ[i]) > 60 ? "..." : "");
    }

    int n = 0;
    while (environ[n]) n++;
    printf("\nTotal env entries: %d\n", n);
    return 0;
}
