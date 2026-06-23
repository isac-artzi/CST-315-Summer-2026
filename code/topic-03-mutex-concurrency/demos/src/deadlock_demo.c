/*
 * deadlock_demo.c — CST-315 · Topic 3 · demos · INSTRUCTOR ONLY
 * Purpose: Two threads, two mutexes, opposite locking order.
 *          Reliably deadlocks within seconds. Press CTRL-C to stop.
 * Build:   make deadlock_demo
 * Run:     ./deadlock_demo
 * Notes:   Portable POSIX. Builds on macOS and Linux.
 *          The deadlock is the point — do not "fix" it.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

static pthread_mutex_t a = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t b = PTHREAD_MUTEX_INITIALIZER;

static void *worker_ab(void *arg) {
    (void)arg;
    for (int i = 0; i < 100; i++) {
        pthread_mutex_lock(&a);
        printf("[AB] locked a, asking for b...\n");
        usleep(50 * 1000);
        pthread_mutex_lock(&b);
        printf("[AB] locked b\n");
        pthread_mutex_unlock(&b);
        pthread_mutex_unlock(&a);
    }
    return NULL;
}
static void *worker_ba(void *arg) {
    (void)arg;
    for (int i = 0; i < 100; i++) {
        pthread_mutex_lock(&b);
        printf("[BA] locked b, asking for a...\n");
        usleep(50 * 1000);
        pthread_mutex_lock(&a);
        printf("[BA] locked a\n");
        pthread_mutex_unlock(&a);
        pthread_mutex_unlock(&b);
    }
    return NULL;
}

int main(void) {
    printf("Two threads compete for mutexes a and b in opposite order.\n");
    printf("Expect a deadlock within a few iterations. CTRL-C to kill.\n");
    pthread_t t1, t2;
    pthread_create(&t1, NULL, worker_ab, NULL);
    pthread_create(&t2, NULL, worker_ba, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    return 0;
}
