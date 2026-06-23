# Topic 3 · Demos — INSTRUCTOR ONLY

> **Do not distribute these programs to students.**

| Binary | What it demonstrates |
|---|---|
| `prodcons_safe` | Bounded-buffer producer/consumer using POSIX **named** semaphores (portable to macOS, which has deprecated `sem_init` for thread-shared semaphores). |
| `prodcons_monitor` | Same scenario using a "monitor" pattern: `pthread_mutex_t` + condition variables. Compare with `prodcons_safe`. |
| `deadlock_demo` | Two threads, two mutexes, opposite locking order. Reliably hangs forever — a clean demonstration of circular wait. |
| `dining_phil` | 4 philosophers, 4 forks. Naive version deadlocks; press CTRL-C to stop. A `-fix` flag enables the "one righty" rule which avoids the cycle. |

## Build & run
```bash
make
./prodcons_safe
./prodcons_monitor
./deadlock_demo            # will hang — kill with CTRL-C, that's the point
./dining_phil              # naive — observe deadlock
./dining_phil -fix         # asymmetric — runs forever without hanging
```

## Notes
All four use pthreads (`-pthread`). Named semaphores in `prodcons_safe` use `/dev/shm`-style paths on Linux and the equivalent on macOS; we clean them up with `sem_unlink`.
