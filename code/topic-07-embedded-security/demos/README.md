# Topic 7 · Demos — INSTRUCTOR ONLY

> **Do not distribute these programs to students.**

| Binary | What it demonstrates |
|---|---|
| `aslr_demo` | Print the address of a stack variable and a heap allocation. Run twice — addresses differ. That's ASLR. |
| `stack_canary_demo` | Two builds of the same program: with and without `-fstack-protector-strong`. Show the canary check on overflow. |
| `seccomp_demo` | Linux only. Install a seccomp filter that allows `write()` and `exit_group()` only. Other syscalls SIGSYS. |
| `setuid_demo` | Print real and effective UIDs. If the binary has setuid bit set, ruid ≠ euid. (Educational; do NOT chmod u+s in production.) |

## Build & run
```bash
make
./aslr_demo
./aslr_demo                                  # run twice; addresses differ
./stack_canary_demo                          # see Makefile for two-build flow
./seccomp_demo                               # Linux only
./setuid_demo                                # ruid/euid printout
```
