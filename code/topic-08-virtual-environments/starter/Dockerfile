# CST-315 — lopeShell container (single-stage, ~120 MB)
#
# STUDENT TODO: convert this to a multi-stage build that ships
# only the compiled binary in the final image.
#
# Goal: drop the image size from ~120 MB to ~80 MB by leaving the
# toolchain (gcc, libc6-dev) in an intermediate "build" stage
# that does not contribute layers to the final image.

FROM debian:bookworm-slim

# Install everything in one layer.
RUN apt-get update && apt-get install -y --no-install-recommends \
        gcc libc6-dev coreutils procps \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /opt/shell
COPY src/lopeShell.c .
RUN gcc -Wall -Wextra -std=c99 -pedantic -D_POSIX_C_SOURCE=200809L \
        -O2 -o lopeShell lopeShell.c

WORKDIR /work
CMD ["/opt/shell/lopeShell"]
