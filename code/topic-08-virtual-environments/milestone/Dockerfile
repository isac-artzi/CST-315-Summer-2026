# CST-315 — final lopeShell container
# Multi-stage build: compile with full toolchain, ship only the binary.

# ---- Stage 1: build ----
FROM debian:bookworm-slim AS build

RUN apt-get update && apt-get install -y --no-install-recommends \
        gcc libc6-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /opt/build
COPY src/lopeShell.c .
RUN gcc -Wall -Wextra -std=c99 -pedantic -D_POSIX_C_SOURCE=200809L \
        -O2 -o lopeShell lopeShell.c

# ---- Stage 2: runtime ----
FROM debian:bookworm-slim AS runtime

# A few utilities for the shell to actually do something inside the container.
RUN apt-get update && apt-get install -y --no-install-recommends \
        coreutils procps \
    && rm -rf /var/lib/apt/lists/*

COPY --from=build /opt/build/lopeShell /usr/local/bin/lopeShell

WORKDIR /work
CMD ["/usr/local/bin/lopeShell"]
