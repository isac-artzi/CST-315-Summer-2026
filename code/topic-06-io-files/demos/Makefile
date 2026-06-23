# Topic 6 · demos · INSTRUCTOR ONLY

CC      ?= cc
CFLAGS   = -Wall -Wextra -std=c99 -pedantic -g -D_POSIX_C_SOURCE=200809L -D_DEFAULT_SOURCE
BINS     = stat_demo fd_table_demo simple_raid0 mmap_io

.PHONY: all clean

all: $(BINS)

stat_demo: src/stat_demo.c
	$(CC) $(CFLAGS) -o $@ $<

fd_table_demo: src/fd_table_demo.c
	$(CC) $(CFLAGS) -o $@ $<

simple_raid0: src/simple_raid0.c
	$(CC) $(CFLAGS) -o $@ $<

mmap_io: src/mmap_io.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(BINS) src/*.o
