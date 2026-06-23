# Topic 2 · Demos — INSTRUCTOR ONLY

> **Do not distribute these programs to students.** The `publish.sh` helper in the repo root skips this directory.

Four small programs that supplement the Topic 2 lesson.

| Binary | What it demonstrates |
|---|---|
| `pthread_basics` | `pthread_create` + `pthread_join`, two threads printing in interleaved order. |
| `zombie_demo` | Parent forks, child exits, parent does NOT wait — `ps` shows `<defunct>`. |
| `signal_demo` | Install `SIGINT` handler (CTRL-C is intercepted, not fatal). |
| `prodcons_busywait` | Racy producer/consumer with no synchronization. Motivates Topic 3. |

## Build & run
```bash
make
./pthread_basics
./zombie_demo
./signal_demo            # press CTRL-C a few times then 'q'
./prodcons_busywait      # observe gaps / lost values
```

## Notes
- All four are pure POSIX C99.
- pthreads requires `-pthread` at link time (Makefile handles it).
