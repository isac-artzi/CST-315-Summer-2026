/*
 * mlfq_sim.c — CST-315 · Topic 5 · demos · INSTRUCTOR ONLY
 * Purpose: 3-level Multilevel Feedback Queue. Processes start at
 *          the highest level. Using a full quantum demotes you;
 *          yielding (simulated I/O) promotes you. Approximates SJF
 *          without knowing burst lengths.
 * Build:   make mlfq_sim
 * Run:     ./mlfq_sim
 */

#include <stdio.h>

#define LEVELS 3

typedef struct {
    int pid, arrival, burst;
    int remaining, level;
    int yields_at;     /* simulated I/O: after running this many ticks, yield */
    int done, completion;
} Proc;

typedef struct { int q[16]; int head, tail, count; } Q;
static void qinit(Q *q){q->head=q->tail=q->count=0;}
static int  qempty(Q *q){return q->count==0;}
static void qpush(Q *q,int x){q->q[q->tail]=x; q->tail=(q->tail+1)%16; q->count++;}
static int  qpop (Q *q){int x=q->q[q->head]; q->head=(q->head+1)%16; q->count--; return x;}

int main(void) {
    Proc p[] = {
        {1,0,8,8,0, 100, 0,0},   /* CPU-bound */
        {2,1,4,4,0,   2, 0,0},   /* I/O-bound: yields every 2 ticks */
        {3,3,5,5,0, 100, 0,0},   /* CPU-bound */
    };
    int n = 3;
    Q levels[LEVELS]; for (int i = 0; i < LEVELS; i++) qinit(&levels[i]);
    int admitted[16] = {0};
    int quantum[LEVELS] = {2, 4, 8};

    int t = 0, completed = 0;
    int cur = -1, ran_this_slice = 0;

    printf("MLFQ (3 levels). CPU-bound demoted, I/O-bound stays high.\n\n");
    while (completed < n && t < 80) {
        for (int i = 0; i < n; i++)
            if (!admitted[i] && p[i].arrival <= t) {
                p[i].level = 0;
                qpush(&levels[0], i);
                admitted[i] = 1;
            }

        if (cur < 0) {
            for (int L = 0; L < LEVELS; L++) {
                if (!qempty(&levels[L])) {
                    cur = qpop(&levels[L]);
                    ran_this_slice = 0;
                    break;
                }
            }
        }

        if (cur >= 0) {
            printf("t=%2d  P%d  lvl=%d  rem=%d\n",
                   t, p[cur].pid, p[cur].level, p[cur].remaining);
            p[cur].remaining--;
            ran_this_slice++;
            t++;
            if (p[cur].remaining == 0) {
                p[cur].completion = t; p[cur].done = 1;
                completed++; cur = -1;
            } else if (ran_this_slice >= p[cur].yields_at) {
                /* I/O — promote one level (cap at top). */
                if (p[cur].level > 0) p[cur].level--;
                qpush(&levels[p[cur].level], cur);
                cur = -1;
            } else if (ran_this_slice >= quantum[p[cur].level]) {
                /* Full quantum — demote. */
                if (p[cur].level < LEVELS - 1) p[cur].level++;
                qpush(&levels[p[cur].level], cur);
                cur = -1;
            }
        } else {
            t++;
        }
    }
    printf("\nCompletion:\n");
    for (int i = 0; i < n; i++)
        printf("  P%d  completion=%d  final-level=%d\n",
               p[i].pid, p[i].completion, p[i].level);
    return 0;
}
