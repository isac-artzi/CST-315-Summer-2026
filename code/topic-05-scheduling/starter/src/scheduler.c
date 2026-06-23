/*
 * scheduler.c — CST-315 · Topic 5 · starter
 * Purpose: Round Robin scheduling simulator.
 *
 * STUDENT TODO: complete the queue ops and the per-tick decision.
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

typedef struct {
    int items[MAX_PROCS];
    int head, tail, count;
} Queue;

static void q_init(Queue *q) { q->head = q->tail = q->count = 0; }
static int  q_empty(Queue *q) { return q->count == 0; }

static void q_push(Queue *q, int x) {
    /* ===========================================================
     * TODO Topic 5 (1 of 2): push x at the tail.
     *
     * - items[tail] = x
     * - tail = (tail + 1) % MAX_PROCS
     * - count++
     * =========================================================== */
    (void)q; (void)x;
}

static int q_pop(Queue *q) {
    /* TODO Topic 5 (1 of 2): pop from the head.
     *
     * - int x = items[head]
     * - head = (head + 1) % MAX_PROCS
     * - count--
     * - return x
     */
    (void)q;
    return -1;
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
    int d[][4] = {{1,0,7,3},{2,2,4,1},{3,4,1,4},{4,5,4,2}};
    int n = sizeof(d)/sizeof(d[0]);
    for (int i = 0; i < n; i++) {
        procs[i].pid=d[i][0]; procs[i].arrival=d[i][1];
        procs[i].burst=d[i][2]; procs[i].priority=d[i][3];
        procs[i].remaining=d[i][2]; procs[i].completion=-1;
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
    int switches = 0, prev_cur = -1;

    printf("scheduler: Round Robin, quantum=%d, %d processes\n\n", quantum, n);
    printf("Gantt trace:\n");

    while (completed < n && t < MAX_TIME) {
        /* ===========================================================
         * TODO Topic 5 (2 of 2): per-tick scheduling decision.
         *
         * Each iteration of this loop is one time unit. Do:
         *
         *   1. Admit any process whose arrival <= t and admitted == 0.
         *      (Push its index into `ready`; mark admitted.)
         *   2. If cur == -1 and ready is non-empty, pop one.
         *      Reset slice_left = quantum. If prev_cur != cur, switches++.
         *   3. If cur >= 0:
         *        printf trace; procs[cur].remaining--; slice_left--; t++.
         *        if remaining == 0: completion=t; completed++; cur=-1.
         *        else if slice_left == 0:
         *           re-admit any new arrivals first,
         *           then q_push(cur), cur = -1.
         *      else:
         *        printf("t=%2d  idle\n", t); t++.
         * =========================================================== */

        /* TODO: replace this stub */
        (void)slice_left; (void)switches; (void)prev_cur; (void)admitted;
        printf("t=%2d  (TODO: implement scheduling)\n", t);
        t++;
        if (t > 5) break;  /* prevent runaway loop in stub */
    }

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
