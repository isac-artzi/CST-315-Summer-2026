# Topic 4 · Demos — INSTRUCTOR ONLY

> **Do not distribute these programs to students.**

| Binary | What it demonstrates |
|---|---|
| `mmap_demo` | Memory-map a file with `mmap`, modify in memory, observe the change on disk. Page cache made visible. |
| `fault_timer` | Time the cost of a first-touch page fault vs. a cached re-access. The cliff between "in RAM" and "must fault in" is dramatic. |
| `bump_allocator` | A 30-line bump allocator. Demonstrates how `malloc` ultimately reduces to "pointer + size" once you don't care about freeing. |
| `belady_anomaly` | Run FIFO with 3 vs 4 frames on a chosen sequence. With 4 frames, FIFO produces MORE faults than with 3 — the textbook anomaly. |

## Build & run
```bash
make
./mmap_demo
./fault_timer
./bump_allocator
./belady_anomaly
```
