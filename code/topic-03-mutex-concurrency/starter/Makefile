# Topic 3 · starter · lopeShell v2

CC      ?= cc
CFLAGS   = -Wall -Wextra -std=c99 -pedantic -g -D_POSIX_C_SOURCE=200809L
BINS     = lopeShell

.PHONY: all clean test

all: $(BINS)

lopeShell: src/lopeShell.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(BINS) src/*.o

test: all
	@./test.sh
