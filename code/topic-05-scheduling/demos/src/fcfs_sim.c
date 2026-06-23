/*
 * fcfs_sim.c — CST-315 · Topic 5 · demos · INSTRUCTOR ONLY
 * Purpose: First-Come First-Served on the default workload.
 *          Same processes as the RR milestone — compare metrics.
 * Build:   make fcfs_sim
 * Run:     ./fcfs_sim
 */

#include <stdio.h>

typedef struct { int pid, arrival, burst; int wait, turn; } Proc;

int main(void) {
    Proc p[] = {{1,0,7,0,0},{2,2,4,0,0},{3,4,1,0,0},{4,5,4,0,0}};
    int n = 4;
    int t = 0;
    printf("FCFS on default workload\n\nGantt:\n");
    for (int i = 0; i < n; i++) {
        if (t < p[i].arrival) t = p[i].arrival;
        printf("  t=%2d..%2d  P%d\n", t, t + p[i].burst, p[i].pid);
        p[i].wait = t - p[i].arrival;
        t += p[i].burst;
        p[i].turn = t - p[i].arrival;
    }
    double sw = 0, st = 0;
    for (int i = 0; i < n; i++) {
        printf("  P%d wait=%d turnaround=%d\n", p[i].pid, p[i].wait, p[i].turn);
        sw += p[i].wait; st += p[i].turn;
    }
    printf("\navg wait=%.2f   avg turnaround=%.2f\n", sw / n, st / n);
    return 0;
}
