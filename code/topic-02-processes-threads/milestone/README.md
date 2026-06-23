# Topic 2 · Milestone — `lopeShell` v1

## Goal
A minimal command-line interpreter that forks a child for each line of input, execs the command, and waits for it. Supports interactive and batch modes plus an `exit` builtin.

## Build
```bash
make
make test
make clean
```

## Run
```bash
./lopeShell                 # interactive
./lopeShell batch.txt       # batch mode (no prompt)
```

Inside the shell:
```
lopeShell> ls -l
lopeShell> echo hello
lopeShell> pwd
lopeShell> exit
```

## Files
- `src/lopeShell.c` — main shell loop, tokenizer, child dispatcher.

## Bridges to
- Lesson: `lessons/Topic_2_Processes_Threads.html`
- Activity 2: `activities/Topic_2_Activity_2.md`
- Project 2 — Command Line Interpreter

## Notes
Pure POSIX. Builds identically on macOS and Linux. This is the seed shell; later topics extend it (Topic 3 adds concurrency, Topic 7 adds input sanitization).
