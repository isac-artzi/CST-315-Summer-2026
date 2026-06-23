# Topic 1 · Milestone — `hello`

## Goal
Smoke-test the Linux/UNIX C toolchain and demonstrate basic environment-variable access. The first deliverable of Project 1.

## Build
```bash
make           # builds ./hello
make clean     # removes binary
make test      # runs the smoke test
```

## Run
```bash
./hello                          # greets $USER, or "world" if unset
./hello Isac                     # greets the argument
USER_OVERRIDE=alice ./hello      # env var overrides
```

## Files
- `src/hello.c` — main program
- `test.sh` — PASS/FAIL smoke test
- `Makefile` — build & test targets

## Bridges to
- Lesson: `lessons/Topic_1_Overview.html`
- Activity 2: `activities/Topic_1_Activity_2.md`
- Project 1 — Part 1 (HelloWorld submission)

## Notes
Pure POSIX. Builds identically on macOS and Linux.
