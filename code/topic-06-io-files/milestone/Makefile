# Topic 6 · milestone · fs

CC      ?= cc
CFLAGS   = -Wall -Wextra -std=c99 -pedantic -g -D_POSIX_C_SOURCE=200809L
BINS     = fs

.PHONY: all clean test

all: $(BINS)

fs: src/fs.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(BINS) src/*.o

test: all
	@./test.sh
