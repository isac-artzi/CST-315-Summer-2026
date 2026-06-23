# Topic 8 · Demos — INSTRUCTOR ONLY

> **Do not distribute these programs to students.**

Three demonstrations, two of them shell scripts because containerization is mostly a configuration concern, not an implementation one.

| Demo | What it demonstrates |
|---|---|
| `namespace_demo` (C) | Linux only. Use `clone()` with `CLONE_NEWUTS \| CLONE_NEWPID` to create a child in fresh UTS and PID namespaces. The child sees PID 1 and can change its hostname without affecting the host. This is what `docker run` does, in 40 lines. |
| `from_scratch_container.sh` | A 20-line shell script that creates a chroot with `unshare`, providing a container-like environment without Docker. Demystifies the term "container." |
| `docker_size_compare.sh` | Builds `lopeshell:singlestage` and `lopeshell:multistage`, then prints both image sizes. Often a 30 MB vs 120 MB story. |

## Build & run
```bash
make
./namespace_demo                # Linux only
./from_scratch_container.sh     # Linux only; may need sudo
./docker_size_compare.sh        # requires Docker
```
