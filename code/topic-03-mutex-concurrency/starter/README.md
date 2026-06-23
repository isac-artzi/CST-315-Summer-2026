# Topic 3 · Starter — `lopeShell` v2

A cumulative skeleton. The Topic 2 shell baseline is already implemented for you (you may replace it with your own version from last week). Three new TODOs are marked for this week's work.

## What you have to do

Open `src/lopeShell.c`. Three TODOs are marked:

1. **`split_commands`**: split a line on `;` into a list of command strings.
2. **`try_builtin`**: handle the `cd` builtin in the parent (no fork).
3. **Multi-command dispatch**: fork *all* commands in one line before waiting; reap them all afterward.

The Topic 2 mechanics (tokenizer, fork-exec-wait body, the input loop, batch mode, `exit` builtin) are already in place as reference.

## Build, run, test
```bash
make
./lopeShell
make test
```

`make test` checks: single command still works, multi-command concurrency, `cd` changes the shell's cwd, CTRL-X exits, and batch mode.

## Hints
- `strtok(line, ";")` splits on semicolons. Beware: `strtok` modifies `line` and stores state internally, so you cannot interleave with another `strtok` call.
- Builtins must run in the parent, otherwise `cd` would only change the child's directory and have no effect.
- For concurrent dispatch: fork all N commands first, store the N PIDs in an array, then call `waitpid` N times. Do NOT fork-and-wait-each.

## Bridges to
- Lesson: `lessons/Topic_3_Mutual_Exclusion_Concurrency.html`
- Project 3 — Improved CLI
