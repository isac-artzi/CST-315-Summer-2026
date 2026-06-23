# Topic 1 · Starter — `hello`

A skeleton for the first deliverable of Project 1.

## What you have to do

Open `src/hello.c`. There is one TODO marked clearly. Implement it so the program greets, in priority order:

1. `argv[1]` if present and non-empty.
2. The `$USER_OVERRIDE` environment variable if set.
3. The `$USER` environment variable if set.
4. The literal string `"world"`.

After the greeting, print one more line exactly:
```
This is CST-315: Operating Systems.
```

## Build, run, test

```bash
make           # builds ./hello
./hello                          # default greeting
./hello Isac                     # greets 'Isac'
USER_OVERRIDE=alice ./hello      # env override
make test                        # runs smoke test (PASS/FAIL)
```

If `make test` is all PASS, you're done with the code part. Push to your GitHub repo.

## Hints
- `getenv("FOO")` returns `NULL` if `FOO` is not set, otherwise a pointer to its value.
- `argv[1]` only exists when `argc > 1`.
- Strings in C are pointers to char; check for empty by comparing the first character to `'\0'`.

## Bridges to
- Lesson: `lessons/Topic_1_Overview.html`
- Activity 2: `activities/Topic_1_Activity_2.md`
- Project 1 — Part 1
