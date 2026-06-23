# Topic 5 · Starter — `scheduler`

A skeleton Round Robin scheduler. The harness (workload loader, time loop frame, metrics printing) is in place. You implement the ready queue and the per-tick scheduling decision.

## What you have to do

Open `src/scheduler.c`. Two TODOs are marked:

1. **Queue ops** — fill in `q_push` and `q_pop` for the ring-buffer ready queue.
2. **Per-tick decision** — admit arrivals, pick a process if idle, run one tick, requeue or terminate on quantum/burst exhaustion.

## Build, run, test
```bash
make
./scheduler 2                # default workload, quantum=2
make test
```

## Hints
- Push at `tail`, pop from `head`. Both wrap modulo `MAX_PROCS`.
- Admit at the **top of each iteration**: any process whose `arrival <= t` and `admitted[i] == 0` joins the queue.
- When the current process's quantum expires (slice_left reaches 0) AND it still has remaining work, re-admit fresh arrivals FIRST, then re-enqueue the current process. This keeps it from cutting in line ahead of just-arrived neighbors.

## Bridges to
- Lesson: `lessons/Topic_5_Processor_Scheduling.html`
- Project 5 — Short-Term Process Scheduler
