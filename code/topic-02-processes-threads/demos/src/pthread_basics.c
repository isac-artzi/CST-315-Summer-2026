/*
 * pthread_basics.c — CST-315 · Topic 2 · demos · INSTRUCTOR ONLY
 * Purpose: Two threads, interleaved output. Smallest possible
 *          demonstration of pthread_create + pthread_join.
 * Build:   make pthread_basics
 * Run:     ./pthread_basics
 * Notes:   Portable POSIX. Builds on macOS and Linux.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

static void *worker(void *arg) {
    const char *label = (const char *)arg;
    for (int i = 0; i < 5; i++) {
        printf("[%s] tick %d\n", label, i);
        usleep(200 * 1000); /* 200 ms */
    }
    return NULL;
}

int main(void) {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, worker, (void *)"A");
    pthread_create(&t2, NULL, worker, (void *)"B");
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("both threads joined.\n");
    return 0;
}
