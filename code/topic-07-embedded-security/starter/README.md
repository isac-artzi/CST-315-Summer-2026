# Topic 7 · Starter — `lopeShell` + `virus` + `scan`

Skeletons for the three Topic 7 binaries. The shell carries over from Topic 3; new TODOs scope the sanitizer, the virus payload, and the scanner walker.

## What you have to do

Three files, one TODO each:

1. **`src/lopeShell.c`** — implement `safe_arg()`: return 0 if the string contains any shell metacharacter (`; & | $ ` < > newline ' "`), 1 otherwise.
2. **`src/virus.c`** — when the user invokes the virus with `rm <file>`, rewrite argv to `echo VIRUS would-have-rm <file>`. Otherwise pass argv through unchanged. NO `system()`.
3. **`src/scan.c`** — implement `scan_dir()`: walk the directory recursively, call `file_has_pattern()` on each regular file, print a WARNING when matched.

## Build, run, test
```bash
make
make test
```

## Hints
- `strpbrk(s, banned)` returns non-NULL if any char in `banned` appears in `s`.
- For the virus: build a `new_argv` array, then `execvp(new_argv[0], new_argv)`.
- For scan: `opendir`/`readdir`/`closedir`. Use `lstat` to detect symlinks and skip them (avoid loops). Recurse into directories.

## Bridges to
- Lesson: `lessons/Topic_7_Embedded_Security.html`
- Assignment 4 — Injection Virus
