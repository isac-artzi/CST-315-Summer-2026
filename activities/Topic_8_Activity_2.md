# Topic 8 · Activity 2 — Containerize Your Shell

**Course:** CST-315 Operating Systems Lecture & Lab
**Duration:** ~40 minutes
**Format:** Individual at your laptop; pairs allowed for debugging

## Aligned objectives
2. Analyze virtualization tools.
3. Draw the architecture of the virtualization environment.
4. Convey complex technical information.

## What you will produce
A Docker image of your shell, built with a Dockerfile, runnable with one command on any Docker host. This becomes the demo platform for Project 7's screencast.

## Setup
- Working Linux/UNIX environment with Docker installed. Verify with `docker --version`.
- Your shell project source. If your project isn't ready, use `lopeShell.c` from Topic 2 as a stand-in.
- A new directory: `~/CST-315/Topic8`.

## Steps

### Part A — Dockerfile concept (5 min)
A Dockerfile is a script that the container engine uses to build an image — a frozen filesystem with an entrypoint command.

### Part B — Build and run (25 min)

1. Copy your shell source into `~/CST-315/Topic8/`.

2. Create `Dockerfile`:

```dockerfile
# Dockerfile for the CST-315 shell project
FROM debian:bookworm-slim

# Install build tools
RUN apt-get update && apt-get install -y --no-install-recommends \
        gcc libc6-dev make \
    && rm -rf /var/lib/apt/lists/*

# Copy source and build
WORKDIR /opt/shell
COPY lopeShell.c .
RUN gcc lopeShell.c -o lopeShell

# Default to running the shell interactively
CMD ["./lopeShell"]
```

3. Build:

```bash
docker build -t lopeshell:demo .
```

4. Run interactively:

```bash
docker run --rm -it lopeshell:demo
```

You should land at `lopeShell>`. Try `ls`, `pwd`, `whoami` — you're inside the container.

5. Time the boot:

```bash
time docker run --rm lopeshell:demo /bin/true
```

Record the real time. Screenshot.

### Part C — Compare to a VM (10 min)

Reference rough boot times:
- Bare-metal program start: ~1 ms
- Container start (`docker run`): ~200–800 ms
- microVM (Firecracker): ~125 ms
- Traditional VM (VirtualBox/ESXi): 5–30 s
- Full VM cold boot: 1+ min

Why does a microVM beat a container? Discuss.

Commit your `Dockerfile` to `~/CST-315/Topic8/` and push.

## Discussion prompts
- The Dockerfile uses `debian:bookworm-slim`. What's in that base image? What changes if you switch to `alpine:3.18`?
- Your container shares the host kernel. What does that mean for a security audit on a multi-tenant cluster?
- For Project 7, would you record your demo from inside a container or from a VM? Defend your choice.

## Stretch goal
Add a second stage to the Dockerfile that builds in one image and copies just the binary into a minimal final image. Compare image sizes with `docker images`.

---
**Companion lesson:** [Topic_8_Virtual_Environments.html](../lessons/Topic_8_Virtual_Environments.html)
**Master visualization:** [Factory_OS_Master.html](../Factory_OS_Master.html)
