# Topic 8 · Starter — final `lopeShell` + container

The shell carries over unchanged from Topic 7's reference. The one new TODO is the **Dockerfile** — convert the provided single-stage version into a multi-stage build that ships only the binary.

## What you have to do

Open `Dockerfile`. It is currently a single-stage build: install gcc, copy source, compile, run. That works but produces a ~120 MB image because the toolchain is baked into the final layer.

Convert it to a multi-stage build:

1. **Stage 1 (`build`)** — start from `debian:bookworm-slim`, install gcc, copy source, compile to `/opt/build/lopeShell`.
2. **Stage 2 (`runtime`)** — start from a fresh `debian:bookworm-slim`, install only `coreutils` and `procps` (no gcc), `COPY --from=build` the binary into `/usr/local/bin/`, and `CMD` it.

Then build and compare:
```bash
docker build -t lopeshell:demo .
docker images lopeshell
docker run --rm -it lopeshell:demo
```

A multi-stage build should drop the image size by ~80 MB.

## Build (native shell)
```bash
make
make test
```

## Hints
- Use `FROM debian:bookworm-slim AS build` to name a stage.
- Use `COPY --from=build /opt/build/lopeShell /usr/local/bin/lopeShell` to pull from the named stage.
- `WORKDIR /work` gives the shell a writable place to operate inside the container.

## Bridges to
- Lesson: `lessons/Topic_8_Virtual_Environments.html`
- Project 7 — Shell Presentation
- Assignment 5 — Assessing Virtualization Software
