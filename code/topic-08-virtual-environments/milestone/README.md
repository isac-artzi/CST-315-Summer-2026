# Topic 8 · Milestone — final `lopeShell` + container

## Goal
The cumulative shell, exactly as it stood at the end of Topic 7, plus a Dockerfile so the demo runs identically on any host with Docker installed.

## Build (native)
```bash
make
make test
```

## Build (container)
```bash
docker build -t lopeshell:demo .
docker run --rm -it lopeshell:demo
```

Inside the container you land at the `lopeShell>` prompt. Try `ls`, `pwd`, `whoami`, `echo hi`, `ls; pwd; whoami` (concurrent), `exit`.

## Files
- `src/lopeShell.c` — final shell (same code as Topic 7 milestone).
- `Dockerfile` — multi-stage build: gcc image to compile, `debian:bookworm-slim` final.

## Bridges to
- Lesson: `lessons/Topic_8_Virtual_Environments.html`
- Activity 2: `activities/Topic_8_Activity_2.md`
- Project 7 — Shell Presentation
- Assignment 5 — Assessing Virtualization Software

## Notes
- The Dockerfile uses a multi-stage build so the final image only contains the shell binary and `/bin/sh`, not the toolchain. Compare image sizes with `docker images`.
- On Apple Silicon Macs, add `--platform linux/amd64` to `docker build` if you need to match the syllabus's Linux/x86 environment.
