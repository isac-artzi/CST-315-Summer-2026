# Topic 4 · demos · INSTRUCTOR ONLY

CC      ?= cc
CFLAGS   = -Wall -Wextra -std=c99 -pedantic -g -D_POSIX_C_SOURCE=200809L -D_DEFAULT_SOURCE
BINS     = mmap_demo fault_timer bump_allocator belady_anomaly

.PHONY: all clean

all: $(BINS)

mmap_demo: src/mmap_demo.c
	$(CC) $(CFLAGS) -o $@ $<

fault_timer: src/fault_timer.c
	$(CC) $(CFLAGS) -o $@ $<

bump_allocator: src/bump_allocator.c
	$(CC) $(CFLAGS) -o $@ $<

belady_anomaly: src/belady_anomaly.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(BINS) src/*.o
