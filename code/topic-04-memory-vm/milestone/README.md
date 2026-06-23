# Topic 4 · Milestone — `pager`

## Goal
A user-space demand-paging simulator. Configurable frame count; LRU page replacement. Reads page references from stdin (one integer per token) or a file passed on the command line.

## Build
```bash
make
make test
make clean
```

## Run
```bash
./pager                                   # reads stdin; defaults to 4 frames
./pager 4 < refs.txt                      # 4 frames, refs from file
echo "1 2 3 4 1 2 5 1 2 3 4 5" | ./pager  # canonical Belady-test sequence
```

Output: a per-step trace plus a final summary (hits, faults, fault rate).

## Files
- `src/pager.c` — simulator with LRU eviction.

## Bridges to
- Lesson: `lessons/Topic_4_Memory_Virtual_Memory.html`
- Activity 2: `activities/Topic_4_Activity_2.md`
- Project 4 — Pager (Virtual Memory Manager)

## Notes
- Pure POSIX. Builds on macOS and Linux.
- The shell from Topic 3 is unchanged this week; you can invoke `pager` from the shell prompt like any other binary.
