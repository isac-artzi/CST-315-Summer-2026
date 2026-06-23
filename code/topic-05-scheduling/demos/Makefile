# Topic 5 · demos · INSTRUCTOR ONLY

CC      ?= cc
CFLAGS   = -Wall -Wextra -std=c99 -pedantic -g -D_POSIX_C_SOURCE=200809L -D_DEFAULT_SOURCE
LDFLAGS  = -pthread
BINS     = fcfs_sim sjf_sim mlfq_sim realtime_demo

.PHONY: all clean

all: $(BINS)

fcfs_sim: src/fcfs_sim.c
	$(CC) $(CFLAGS) -o $@ $<

sjf_sim: src/sjf_sim.c
	$(CC) $(CFLAGS) -o $@ $<

mlfq_sim: src/mlfq_sim.c
	$(CC) $(CFLAGS) -o $@ $<

realtime_demo: src/realtime_demo.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(BINS) src/*.o
