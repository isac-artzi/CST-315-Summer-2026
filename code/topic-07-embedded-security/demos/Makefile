# Topic 7 · demos · INSTRUCTOR ONLY

CC      ?= cc
CFLAGS   = -Wall -Wextra -std=c99 -pedantic -g -D_POSIX_C_SOURCE=200809L -D_DEFAULT_SOURCE
BINS     = aslr_demo stack_canary_demo seccomp_demo setuid_demo

.PHONY: all clean

all: $(BINS)

aslr_demo: src/aslr_demo.c
	$(CC) $(CFLAGS) -o $@ $<

# Build with stack protector ON
stack_canary_demo: src/stack_canary_demo.c
	$(CC) $(CFLAGS) -fstack-protector-strong -o $@ $<

seccomp_demo: src/seccomp_demo.c
	$(CC) $(CFLAGS) -o $@ $<

setuid_demo: src/setuid_demo.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(BINS) src/*.o
