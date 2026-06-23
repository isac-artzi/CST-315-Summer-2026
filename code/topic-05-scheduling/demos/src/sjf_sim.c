/*
 * sjf_sim.c — CST-315 · Topic 5 · demos · INSTRUCTOR ONLY
 * Purpose: Shortest Job First (non-preemptive) on the default
 *          workload. Among processes that have arrived, pick
 *          the shortest burst. Compare with FCFS and RR.
 * Build:   make sjf_sim
 * Run:     ./sjf_sim
 */

#include <stdio.h>

typedef struct { int pid, arrival, burst, done; int wait, turn; } Proc;

int main(void) {
    Proc p[] = {
        {1,0,7,0,0,0},{2,2,4,0,0,0},{3,4,1,0,0,0},{4,5,4,0,0,0}
    };
    int n = 4, t = 0, completed = 0;
    printf("SJF (non-preemptive) on default workload\n\nGantt:\n");
    while (completed < n) {
        int pick = -1, best = 1<<30;
        for (int i = 0; i < n; i++)
            if (!p[i].done && p[i].arrival <= t && p[i].burst < best) {
                pick = i; best = p[i].burst;
            }
        if (pick < 0) { t++; continue; }
        printf("  t=%2d..%2d  P%d\n", t, t + p[pick].burst, p[pick].pid);
        p[pick].wait = t - p[pick].arrival;
        t += p[pick].burst;
        p[pick].turn = t - p[pick].arrival;
        p[pick].done = 1;
        completed++;
    }
    double sw = 0, st = 0;
    for (int i = 0; i < n; i++) {
        printf("  P%d wait=%d turnaround=%d\n", p[i].pid, p[i].wait, p[i].turn);
        sw += p[i].wait; st += p[i].turn;
    }
    printf("\navg wait=%.2f   avg turnaround=%.2f\n", sw / n, st / n);
    return 0;
}
