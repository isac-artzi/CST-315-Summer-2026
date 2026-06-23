# Topic 5 · Demos — INSTRUCTOR ONLY

> **Do not distribute these programs to students.**

| Binary | What it demonstrates |
|---|---|
| `fcfs_sim` | First-Come First-Served on the same default workload as the milestone. Convoy effect made visible. |
| `sjf_sim` | Shortest Job First (non-preemptive). Lower average waiting than FCFS on this workload. |
| `mlfq_sim` | Multilevel Feedback Queue with 3 priority levels. CPU-bound processes get demoted; I/O-bound get promoted. |
| `realtime_demo` | Linux only: switch a thread to `SCHED_FIFO` priority and observe behavior vs SCHED_OTHER. Skips on macOS. |

## Build & run
```bash
make
./fcfs_sim
./sjf_sim
./mlfq_sim
./realtime_demo            # may need sudo on some systems for real-time priority
```
