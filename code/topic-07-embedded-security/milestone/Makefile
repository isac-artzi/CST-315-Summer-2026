# Topic 7 · milestone · lopeShell + virus + scan

CC      ?= cc
CFLAGS   = -Wall -Wextra -std=c99 -pedantic -g -D_POSIX_C_SOURCE=200809L
BINS     = lopeShell virus scan

.PHONY: all clean test

all: $(BINS)

lopeShell: src/lopeShell.c
	$(CC) $(CFLAGS) -o $@ $<

virus: src/virus.c
	$(CC) $(CFLAGS) -o $@ $<

scan: src/scan.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(BINS) src/*.o

test: all
	@./test.sh
