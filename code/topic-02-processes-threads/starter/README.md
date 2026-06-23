# Topic 2 · Starter — `lopeShell` v1

A skeleton command-line interpreter for Project 2. You will implement the fork-exec-wait loop.

## What you have to do

Open `src/lopeShell.c`. Two TODOs are marked:

1. **Tokenize**: split the input line into argv on whitespace.
2. **Run the command**: fork, child execs, parent waits.

The harness (interactive/batch mode toggle, prompt, input read, blank-line skip, `exit` builtin) is already written.

## Build, run, test
```bash
make
./lopeShell                       # interactive
./lopeShell my_batch.txt          # batch
make test                         # PASS/FAIL smoke test
```

`make test` checks: interactive `echo`, interactive `pwd`, unknown-command error, and two-line batch mode.

## Hints
- `fork()` returns `0` in the child, the child's PID in the parent (or `<0` on error).
- `execvp(argv[0], argv)` searches `$PATH`. It only returns on failure.
- `waitpid(pid, &status, 0)` blocks until that specific child exits.
- Don't forget `_exit(127);` after `execvp` so a failed exec doesn't fall through and become a second shell.

## Bridges to
- Lesson: `lessons/Topic_2_Processes_Threads.html`
- Activity 2: `activities/Topic_2_Activity_2.md`
- Project 2 — Command Line Interpreter
