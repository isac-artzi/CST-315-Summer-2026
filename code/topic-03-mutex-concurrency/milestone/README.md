# Topic 3 · Milestone — `lopeShell` v2

## Goal
Extend Topic 2's shell to handle semicolon-separated multi-command lines concurrently, add a `cd` builtin, and exit cleanly on CTRL-X.

## Build
```bash
make
make test
make clean
```

## Run
```bash
./lopeShell
lopeShell> ls; pwd; whoami       # three children run concurrently
lopeShell> cd /tmp; pwd          # cd is a builtin (per-shell state)
lopeShell> <CTRL-X>              # exits cleanly
```

## Files
- `src/lopeShell.c` — extended shell with concurrent dispatch, builtins, signal setup.

## Bridges to
- Lesson: `lessons/Topic_3_Mutual_Exclusion_Concurrency.html`
- Activity 2: `activities/Topic_3_Activity_2.md`
- Project 3 — Improved CLI

## Notes
- CTRL-X is bound by setting up a SIGQUIT handler and remapping; the simpler approach used here is to install a SIGINT handler that prints a hint, and treat CTRL-X (ASCII 0x18) as an in-line exit token. Both methods are acceptable for Project 3.
- POSIX-only. Builds on macOS and Linux.
