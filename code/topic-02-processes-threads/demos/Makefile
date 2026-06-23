# Topic 2 · demos · INSTRUCTOR ONLY

CC      ?= cc
CFLAGS   = -Wall -Wextra -std=c99 -pedantic -g -D_POSIX_C_SOURCE=200809L -D_DEFAULT_SOURCE
LDFLAGS  = -pthread
BINS     = pthread_basics zombie_demo signal_demo prodcons_busywait

.PHONY: all clean

all: $(BINS)

pthread_basics: src/pthread_basics.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

zombie_demo: src/zombie_demo.c
	$(CC) $(CFLAGS) -o $@ $<

signal_demo: src/signal_demo.c
	$(CC) $(CFLAGS) -o $@ $<

prodcons_busywait: src/prodcons_busywait.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(BINS) src/*.o
