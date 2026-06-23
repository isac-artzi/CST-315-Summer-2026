/*
 * scan.c — CST-315 · Topic 7 · milestone
 * Purpose: Pattern-based directory scanner. Walks a directory tree
 *          and flags files that contain known-suspicious substrings.
 *          The detector half of Assignment 4.
 * Build:   make scan
 * Run:     ./scan <directory>
 * Notes:   POSIX. Builds on macOS and Linux. Skips symlinks to
 *          avoid loops.
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

static void scan_dir(const char *path) {
    DIR *d = opendir(path);
    if (!d) { perror(path); return; }
    struct dirent *ent;
    while ((ent = readdir(d)) != NULL) {
        if (ent->d_name[0] == '.') continue;
        char full[2048];
        snprintf(full, sizeof(full), "%s/%s", path, ent->d_name);

        struct stat st;
        if (lstat(full, &st) != 0) continue;
        if (S_ISLNK(st.st_mode)) continue;       /* avoid symlink loops */
        if (S_ISDIR(st.st_mode)) { scan_dir(full); continue; }
        if (!S_ISREG(st.st_mode)) continue;

        const char *found = NULL;
        if (file_has_pattern(full, &found)) {
            printf("WARNING: %s contains \"%s\"\n", full, found);
            flagged++;
        }
    }
    closedir(d);
}

int main(int argc, char **argv) {
    const char *target = (argc > 1) ? argv[1] : ".";
    scan_dir(target);
    printf("\nscan complete: %d file(s) flagged\n", flagged);
    return flagged > 0 ? 0 : 0;
}
