/*
 * scheduler.c — CST-315 · Topic 5 · milestone
 * Purpose: Round Robin scheduling simulator. Reads a workload
 *          (pid arrival burst priority per line) and prints a
 *          Gantt-style trace plus metrics.
 * Build:   make
 * Run:     ./scheduler [quantum] [workload-file]
 *          ./scheduler                 # default workload, q=2
 *          ./scheduler 3 myload.txt    # q=3, custom workload
 * Notes:   POSIX. Builds on macOS and Linux.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PROCS 32
#define MAX_TIME  256

typedef struct {
    int pid, arrival, burst, priority;
    int remaining;
    int completion;
} Proc;

/* Simple ring-buffer queue of indices into procs[]. */
typedef struct {
    int items[MAX_PROCS];
    int head, tail, count;
} Queue;

static void q_init(Queue *q) { q->head = q->tail = q->count = 0; }
static int  q_empty(Queue *q) { return q->count == 0; }
static void q_push(Queue *q, int x) {
    q->items[q->tail] = x;
    q->tail = (q->tail + 1) % MAX_PROCS;
    q->count++;
}
static int  q_pop(Queue *q) {
    int x = q->items[q->head];
    q->head = (q->head + 1) % MAX_PROCS;
    q->count--;
    return x;
}

static int load_workload(FILE *f, Proc procs[]) {
    int n = 0;
    while (n < MAX_PROCS && fscanf(f, "%d %d %d %d",
                                   &procs[n].pid, &procs[n].arrival,
                                   &procs[n].burst, &procs[n].priority) == 4) {
        procs[n].remaining  = procs[n].burst;
        procs[n].completion = -1;
        n++;
    }
    return n;
}
static int default_workload(Proc procs[]) {
    int defaults[][4] = {
        {1, 0, 7, 3}, {2, 2, 4, 1}, {3, 4, 1, 4}, {4, 5, 4, 2}
    };
    int n = sizeof(defaults) / sizeof(defaults[0]);
    for (int i = 0; i < n; i++) {
        procs[i].pid       = defaults[i][0];
        procs[i].arrival   = defaults[i][1];
        procs[i].burst     = defaults[i][2];
        procs[i].priority  = defaults[i][3];
        procs[i].remaining = defaults[i][2];
        procs[i].completion = -1;
    }
    return n;
}

int main(int argc, char **argv) {
    int quantum = (argc > 1) ? atoi(argv[1]) : 2;
    if (quantum < 1) quantum = 1;

    Proc procs[MAX_PROCS];
    int n;
    if (argc > 2) {
        FILE *f = fopen(argv[2], "r");
        if (!f) { perror(argv[2]); return 1; }
        n = load_workload(f, procs);
        fclose(f);
    } else {
        n = default_workload(procs);
    }

    if (n == 0) { fprintf(stderr, "scheduler: no processes\n"); return 1; }

    Queue ready; q_init(&ready);
    int admitted[MAX_PROCS] = {0};
    int t = 0, completed = 0;
    int cur = -1, slice_left = 0;
    int switches = 0;
    int prev_cur = -1;

    printf("scheduler: Round Robin, quantum=%d, %d processes\n\n", quantum, n);
    printf("Gantt trace:\n");

    while (completed < n && t < MAX_TIME) {
        /* Admit new arrivals at time t. */
        for (int i = 0; i < n; i++) {
            if (!admitted[i] && procs[i].arrival <= t) {
                q_push(&ready, i);
                admitted[i] = 1;
            }
        }
        /* Pick next if idle. */
        if (cur < 0 && !q_empty(&ready)) {
            cur = q_pop(&ready);
            slice_left = quantum;
            if (prev_cur != cur) switches++;
            prev_cur = cur;
        }

        if (cur >= 0) {
            printf("t=%2d  P%d  remaining=%d\n", t, procs[cur].pid, procs[cur].remaining);
            procs[cur].remaining--;
            slice_left--;
            t++;

            if (procs[cur].remaining == 0) {
                procs[cur].completion = t;
                completed++;
                cur = -1;
            } else if (slice_left == 0) {
                /* Re-admit any arrivals BEFORE re-enqueueing current,
                 * so they queue ahead in the next slice. */
                for (int i = 0; i < n; i++) {
                    if (!admitted[i] && procs[i].arrival <= t) {
                        q_push(&ready, i);
                        admitted[i] = 1;
                    }
                }
                q_push(&ready, cur);
                cur = -1;
            }
        } else {
            printf("t=%2d  idle\n", t);
            t++;
        }
    }

    /* Metrics. */
    double sum_wait = 0, sum_turn = 0;
    printf("\nPer-process completion:\n");
    for (int i = 0; i < n; i++) {
        int turn = procs[i].completion - procs[i].arrival;
        int wait = turn - procs[i].burst;
        sum_wait += wait; sum_turn += turn;
        printf("  P%d  arrival=%d  burst=%d  completion=%d  wait=%d  turnaround=%d\n",
               procs[i].pid, procs[i].arrival, procs[i].burst,
               procs[i].completion, wait, turn);
    }
    printf("\navg waiting:     %.2f\n", sum_wait / n);
    printf("avg turnaround:  %.2f\n", sum_turn / n);
    printf("throughput:      %.3f procs/unit\n", (double)n / t);
    printf("context switches: %d\n", switches);
    return 0;
}
