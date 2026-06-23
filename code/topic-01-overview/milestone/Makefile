# Topic 1 · milestone · Hello World smoke test
# Portable to macOS (BSD) and Linux. POSIX only.

CC      ?= cc
CFLAGS   = -Wall -Wextra -std=c99 -pedantic -g -D_POSIX_C_SOURCE=200809L
BINS     = hello

.PHONY: all clean test

all: $(BINS)

hello: src/hello.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(BINS) src/*.o

test: all
	@./test.sh
