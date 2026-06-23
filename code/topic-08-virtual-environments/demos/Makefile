# Topic 8 · demos · INSTRUCTOR ONLY

CC      ?= cc
CFLAGS   = -Wall -Wextra -std=c99 -pedantic -g -D_GNU_SOURCE
BINS     = namespace_demo

.PHONY: all clean

all: $(BINS)

namespace_demo: src/namespace_demo.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(BINS) src/*.o
