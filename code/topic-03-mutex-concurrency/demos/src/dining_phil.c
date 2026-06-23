/*
 * dining_phil.c — CST-315 · Topic 3 · demos · INSTRUCTOR ONLY
 * Purpose: Dining philosophers with 4 seats. Naive version
 *          deadlocks; the `-fix` flag uses the "one philosopher
 *          picks up the right fork first" rule which breaks the
 *          symmetry and prevents deadlock.
 * Build:   make dining_phil
 * Run:     ./dining_phil         (naive — will deadlock; CTRL-C to stop)
 *          ./dining_phil -fix    (asymmetric — runs indefinitely)
 * Notes:   Portable POSIX. Builds on macOS and Linux.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define N 4

static pthread_mutex_t forks[N];
static int g_fix = 0;

static void think(int id) { (void)id; usleep(rand() % 100 * 1000); }
static void eat(int id, int round) {
    printf("philosopher %d eats (round %d)\n", id, round);
    usleep(rand() % 100 * 1000);
}

static void *philosopher(void *arg) {
    int id = (int)(long)arg;
    int left  = id;
    int right = (id + 1) % N;
    /* Fix: the last philosopher picks up the right fork first. */
    int first  = (g_fix && id == N - 1) ? right : left;
    int second = (g_fix && id == N - 1) ? left  : right;

    for (int round = 1; round <= 1000; round++) {
        think(id);
        pthread_mutex_lock(&forks[first]);
        printf("philosopher %d picked up fork %d\n", id, first);
        pthread_mutex_lock(&forks[second]);
        printf("philosopher %d picked up fork %d\n", id, second);
        eat(id, round);
        pthread_mutex_unlock(&forks[second]);
        pthread_mutex_unlock(&forks[first]);
    }
    return NULL;
}

int main(int argc, char **argv) {
    if (argc > 1 && strcmp(argv[1], "-fix") == 0) g_fix = 1;
    printf("dining philosophers (N=%d, fix=%s)\n", N, g_fix ? "ON" : "OFF");
    for (int i = 0; i < N; i++) pthread_mutex_init(&forks[i], NULL);
    pthread_t threads[N];
    for (long i = 0; i < N; i++) pthread_create(&threads[i], NULL, philosopher, (void *)i);
    for (int i = 0; i < N; i++) pthread_join(threads[i], NULL);
    return 0;
}
