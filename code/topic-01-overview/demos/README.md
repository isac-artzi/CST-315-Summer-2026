# Topic 1 · Demos — INSTRUCTOR ONLY

> **Do not distribute these programs to students.** They are for in-class
> demonstration. The `publish.sh` helper in the repo root deliberately
> skips this directory.

Four small programs that supplement the Topic 1 lesson.

| Binary | What it demonstrates | Use in lecture |
|---|---|---|
| `pipe_demo` | `pipe()` + `fork()` + `dup2()` | "What goes on under `ls -l \| wc -l`?" Previews fork/exec/wait for Topic 2 and the pipe machinery for the shell. |
| `env_demo` | `getenv` / `setenv` / `environ` array | "How does the shell pass `$PATH` to a child?" |
| `procfs_walker` | reads `/proc/self/status` | Visualizes the PCB before the term "PCB" is even introduced. Linux only. |
| `whoami_clone` | `getuid` + `getpwuid` | Reimplements `whoami` from scratch — same idea your future shell builtins will use. |

## Build & run
```bash
make           # builds all four
./pipe_demo
./env_demo
./procfs_walker
./whoami_clone
```

`make clean` removes binaries. No tests; these are demonstration code.

## Notes
- All four are pure POSIX C99.
- `procfs_walker` is Linux only; on macOS it prints a friendly skip message and a suggested alternative (`ps -o ...`).
