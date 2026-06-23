/*
 * prodcons_busywait.c — CST-315 · Topic 2 · demos · INSTRUCTOR ONLY
 * Purpose: Producer/consumer with NO synchronization. Two threads
 *          share a single int. The producer increments; the consumer
 *          prints whenever it sees a new value. Observe missed values
 *          and inconsistent reads. This is the motivating problem for
 *          Topic 3 (semaphores, monitors).
 * Build:   make prodcons_busywait
 * Run:     ./prodcons_busywait
 * Notes:   Portable POSIX. Builds on macOS and Linux.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

static volatile int buffer = 0;
static volatile int done   = 0;

static void *producer(void *arg) {
    (void)arg;
    for (int i = 1; i <= 100; i++) {
        buffer = i;
        usleep(800);
    }
    done = 1;
    return NULL;
}
static void *consumer(void *arg) {
    (void)arg;
    int last = 0;
    int missed = 0;
    while (!done) {
        int v = buffer;
        if (v != last) {
            int gap = v - last - 1;
            if (gap > 0) missed += gap;
            printf("got %3d (gap %d)\n", v, gap);
            last = v;
        }
        usleep(800);
    }
    printf("\nconsumer done. items missed: %d (out of 100)\n", missed);
    return NULL;
}

int main(void) {
    pthread_t p, c;
    pthread_create(&p, NULL, producer, NULL);
    pthread_create(&c, NULL, consumer, NULL);
    pthread_join(p, NULL);
    pthread_join(c, NULL);
    return 0;
}
