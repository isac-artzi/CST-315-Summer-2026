# Topic 3 · demos · INSTRUCTOR ONLY

CC      ?= cc
CFLAGS   = -Wall -Wextra -std=c99 -pedantic -g -D_POSIX_C_SOURCE=200809L -D_DEFAULT_SOURCE
LDFLAGS  = -pthread
BINS     = prodcons_safe prodcons_monitor deadlock_demo dining_phil

.PHONY: all clean

all: $(BINS)

prodcons_safe: src/prodcons_safe.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

prodcons_monitor: src/prodcons_monitor.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

deadlock_demo: src/deadlock_demo.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

dining_phil: src/dining_phil.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -f $(BINS) src/*.o
