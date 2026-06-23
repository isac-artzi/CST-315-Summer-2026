# Topic 5 · Milestone — `scheduler`

## Goal
A Round Robin scheduling simulator. Reads a workload (one process per line: `pid arrival burst priority`), runs RR with a chosen quantum, and prints both a Gantt-style trace and the standard metrics.

## Build
```bash
make
make test
make clean
```

## Run
```bash
./scheduler                  # default workload, quantum=2
./scheduler 3                # default workload, quantum=3
./scheduler 2 workload.txt   # custom workload, quantum=2
```

A line of `workload.txt` looks like:
```
1 0 7 3
2 2 4 1
3 4 1 4
4 5 4 2
```

## Files
- `src/scheduler.c` — RR engine, queue management, metrics.

## Bridges to
- Lesson: `lessons/Topic_5_Processor_Scheduling.html`
- Activity 2: `activities/Topic_5_Activity_2.md`
- Project 5 — Short-Term Process Scheduler

## Notes
Pure POSIX. Builds on macOS and Linux. Extend in Project 5 with `procs`, `procs -a`, `procs -a -si`, and a priority modification command.
