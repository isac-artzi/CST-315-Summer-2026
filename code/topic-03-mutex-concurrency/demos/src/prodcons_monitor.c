/*
 * prodcons_monitor.c — CST-315 · Topic 3 · demos · INSTRUCTOR ONLY
 * Purpose: Same producer/consumer scenario as prodcons_safe.c,
 *          but using a "monitor" pattern: a struct wrapping the
 *          shared buffer with one mutex and two condition variables.
 *          Side-by-side comparison is the point.
 * Build:   make prodcons_monitor
 * Run:     ./prodcons_monitor
 * Notes:   Portable POSIX. Builds on macOS and Linux.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 5
#define ITEMS 30

/* The "monitor" — shared state plus the lock and condition variables
 * that protect it. Operations always lock the mutex first. */
typedef struct {
    int buf[N];
    int in, out, count;
    pthread_mutex_t lock;
    pthread_cond_t  not_full;
    pthread_cond_t  not_empty;
} BoundedBuffer;

static void bb_init(BoundedBuffer *b) {
    b->in = b->out = b->count = 0;
    pthread_mutex_init(&b->lock, NULL);
    pthread_cond_init(&b->not_full, NULL);
    pthread_cond_init(&b->not_empty, NULL);
}
static void bb_destroy(BoundedBuffer *b) {
    pthread_mutex_destroy(&b->lock);
    pthread_cond_destroy(&b->not_full);
    pthread_cond_destroy(&b->not_empty);
}
static void bb_put(BoundedBuffer *b, int v) {
    pthread_mutex_lock(&b->lock);
    while (b->count == N) pthread_cond_wait(&b->not_full, &b->lock);
    b->buf[b->in] = v;
    b->in = (b->in + 1) % N;
    b->count++;
    pthread_cond_signal(&b->not_empty);
    pthread_mutex_unlock(&b->lock);
}
static int bb_get(BoundedBuffer *b) {
    pthread_mutex_lock(&b->lock);
    while (b->count == 0) pthread_cond_wait(&b->not_empty, &b->lock);
    int v = b->buf[b->out];
    b->out = (b->out + 1) % N;
    b->count--;
    pthread_cond_signal(&b->not_full);
    pthread_mutex_unlock(&b->lock);
    return v;
}

static BoundedBuffer monitor;

static void *producer(void *arg) {
    (void)arg;
    for (int i = 1; i <= ITEMS; i++) {
        bb_put(&monitor, i);
        printf("produced %d\n", i);
    }
    return NULL;
}
static void *consumer(void *arg) {
    (void)arg;
    for (int i = 0; i < ITEMS; i++) {
        int v = bb_get(&monitor);
        printf("                  consumed %d\n", v);
    }
    return NULL;
}

int main(void) {
    bb_init(&monitor);
    pthread_t p, c;
    pthread_create(&p, NULL, producer, NULL);
    pthread_create(&c, NULL, consumer, NULL);
    pthread_join(p, NULL);
    pthread_join(c, NULL);
    bb_destroy(&monitor);
    return 0;
}
