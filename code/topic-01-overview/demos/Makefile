# Topic 1 · demos · INSTRUCTOR ONLY
# Build four small demonstration programs.

CC      ?= cc
CFLAGS   = -Wall -Wextra -std=c99 -pedantic -g -D_POSIX_C_SOURCE=200809L
BINS     = pipe_demo env_demo procfs_walker whoami_clone

.PHONY: all clean

all: $(BINS)

pipe_demo: src/pipe_demo.c
	$(CC) $(CFLAGS) -o $@ $<

env_demo: src/env_demo.c
	$(CC) $(CFLAGS) -o $@ $<

procfs_walker: src/procfs_walker.c
	$(CC) $(CFLAGS) -o $@ $<

whoami_clone: src/whoami_clone.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(BINS) src/*.o
