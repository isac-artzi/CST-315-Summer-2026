/*
 * scan.c — CST-315 · Topic 7 · starter
 *
 * STUDENT TODO: implement scan_dir().
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

static const char *patterns[] = {
    "system(",
    "execvp(\"/bin/sh\"",
    "rm -rf",
    "chmod 777",
    "curl http", "wget http",
    "| sh", "| bash",
    NULL
};

static int file_has_pattern(const char *path, const char **found) {
    FILE *f = fopen(path, "r");
    if (!f) return 0;
    char line[2048];
    while (fgets(line, sizeof(line), f)) {
        for (int i = 0; patterns[i]; i++) {
            if (strstr(line, patterns[i])) {
                *found = patterns[i];
                fclose(f);
                return 1;
            }
        }
    }
    fclose(f);
    return 0;
}

static int flagged = 0;

/* ===========================================================
 * TODO Topic 7: implement recursive directory walker.
 *
 *   1. opendir(path); on failure perror, return.
 *   2. For each entry (skip names starting with '.'):
 *      build the full path with snprintf.
 *      lstat() it. Skip symlinks (S_ISLNK) to avoid loops.
 *      If directory: recurse.
 *      If regular file (S_ISREG): call file_has_pattern;
 *        on hit, printf WARNING and flagged++.
 *   3. closedir.
 * =========================================================== */
static void scan_dir(const char *path) {
    /* TODO: replace this stub */
    (void)path;
    printf("TODO: implement scan_dir\n");
}

int main(int argc, char **argv) {
    const char *target = (argc > 1) ? argv[1] : ".";
    scan_dir(target);
    printf("\nscan complete: %d file(s) flagged\n", flagged);
    return 0;
}
