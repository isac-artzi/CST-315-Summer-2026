# Topic 4 · starter · pager

CC      ?= cc
CFLAGS   = -Wall -Wextra -std=c99 -pedantic -g -D_POSIX_C_SOURCE=200809L
BINS     = pager

.PHONY: all clean test

all: $(BINS)

pager: src/pager.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(BINS) src/*.o

test: all
	@./test.sh
