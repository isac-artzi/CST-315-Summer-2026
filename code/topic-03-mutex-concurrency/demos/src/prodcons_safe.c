/*
 * prodcons_safe.c — CST-315 · Topic 3 · demos · INSTRUCTOR ONLY
 * Purpose: Bounded-buffer producer/consumer with POSIX named
 *          semaphores (portable to macOS, where sem_init with
 *          pshared=0 is deprecated).
 * Build:   make prodcons_safe
 * Run:     ./prodcons_safe
 * Notes:   Uses sem_open / sem_close / sem_unlink. Cleanup is
 *          attempted on exit; if a previous run left stale
 *          semaphores they are unlinked at startup.
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define N 5
#define ITEMS 30

static int  buf[N];
static int  in_idx = 0, out_idx = 0;
static sem_t *s_empty, *s_full, *s_mutex;

static void *producer(void *arg) {
    (void)arg;
    for (int i = 1; i <= ITEMS; i++) {
        sem_wait(s_empty);
        sem_wait(s_mutex);
        buf[in_idx] = i;
        in_idx = (in_idx + 1) % N;
        printf("produced %d\n", i);
        sem_post(s_mutex);
        sem_post(s_full);
    }
    return NULL;
}
static void *consumer(void *arg) {
    (void)arg;
    for (int i = 0; i < ITEMS; i++) {
        sem_wait(s_full);
        sem_wait(s_mutex);
        int v = buf[out_idx];
        out_idx = (out_idx + 1) % N;
        printf("                  consumed %d\n", v);
        sem_post(s_mutex);
        sem_post(s_empty);
    }
    return NULL;
}

int main(void) {
    /* Pre-cleanup in case of stale semaphores. */
    sem_unlink("/cst315_empty");
    sem_unlink("/cst315_full");
    sem_unlink("/cst315_mutex");

    s_empty = sem_open("/cst315_empty", O_CREAT, 0600, N);
    s_full  = sem_open("/cst315_full",  O_CREAT, 0600, 0);
    s_mutex = sem_open("/cst315_mutex", O_CREAT, 0600, 1);
    if (s_empty == SEM_FAILED || s_full == SEM_FAILED || s_mutex == SEM_FAILED) {
        perror("sem_open"); return 1;
    }

    pthread_t p, c;
    pthread_create(&p, NULL, producer, NULL);
    pthread_create(&c, NULL, consumer, NULL);
    pthread_join(p, NULL);
    pthread_join(c, NULL);

    sem_close(s_empty); sem_close(s_full); sem_close(s_mutex);
    sem_unlink("/cst315_empty");
    sem_unlink("/cst315_full");
    sem_unlink("/cst315_mutex");
    return 0;
}
