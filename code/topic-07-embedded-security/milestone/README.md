# Topic 7 · Milestone — `lopeShell` + `virus` + `scan`

## Goal
Add a sanitization pass to the shell, then build Assignment 4's two pieces side-by-side: a command-injection virus and a pattern-based detector.

## Build
```bash
make
make test
make clean
```

## Run
```bash
./lopeShell                              # tries 'rm a;b' — rejected by sanitizer
./virus rm safe_file                     # virus rewrites argv before exec'ing
./scan test_corpus/                      # scanner walks dir and flags suspects
```

## Files
- `src/lopeShell.c` — shell with sanitization (rejects `; & | $ ` in args).
- `src/virus.c` — minimal injection virus (no `system()`).
- `src/scan.c` — directory walker + pattern matcher.

## Bridges to
- Lesson: `lessons/Topic_7_Embedded_Security.html`
- Activity 2: `activities/Topic_7_Activity_2.md`
- Assignment 4 — Injection Virus

## Notes
- Pure POSIX. Builds on macOS and Linux.
- The virus is benign: it rewrites `rm <file>` to `echo` to make the demonstration safe. Real Assignment 4 work uses a different payload.
