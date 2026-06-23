/*
 * belady_anomaly.c — CST-315 · Topic 4 · demos · INSTRUCTOR ONLY
 * Purpose: Demonstrate Belady's anomaly with FIFO replacement.
 *          The reference string 1 2 3 4 1 2 5 1 2 3 4 5 produces
 *          MORE faults with 4 frames than with 3 under FIFO — a
 *          textbook surprise. LRU is anomaly-free.
 * Build:   make belady_anomaly
 * Run:     ./belady_anomaly
 * Notes:   Portable POSIX.
 */

#include <stdio.h>
#include <stdlib.h>

static int fifo_faults(int frames_n, const int refs[], int nrefs) {
    int frames[16];
    int load_time[16];
    for (int i = 0; i < frames_n; i++) { frames[i] = -1; load_time[i] = -1; }

    int faults = 0;
    for (int t = 0; t < nrefs; t++) {
        int p = refs[t];
        /* Hit? */
        int hit = 0;
        for (int i = 0; i < frames_n; i++) if (frames[i] == p) { hit = 1; break; }
        if (hit) continue;
        faults++;
        /* Free frame? */
        int filled = -1;
        for (int i = 0; i < frames_n; i++) if (frames[i] < 0) { filled = i; break; }
        if (filled >= 0) {
            frames[filled] = p; load_time[filled] = t;
            continue;
        }
        /* Evict oldest-loaded. */
        int victim = 0;
        for (int i = 1; i < frames_n; i++)
            if (load_time[i] < load_time[victim]) victim = i;
        frames[victim] = p; load_time[victim] = t;
    }
    return faults;
}

int main(void) {
    int refs[] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};
    int n = sizeof(refs) / sizeof(refs[0]);

    int f3 = fifo_faults(3, refs, n);
    int f4 = fifo_faults(4, refs, n);
    int f5 = fifo_faults(5, refs, n);

    printf("Reference string (%d refs): 1 2 3 4 1 2 5 1 2 3 4 5\n\n", n);
    printf("FIFO with 3 frames: %d faults\n", f3);
    printf("FIFO with 4 frames: %d faults  %s\n",
           f4, f4 > f3 ? "← BELADY'S ANOMALY" : "");
    printf("FIFO with 5 frames: %d faults\n\n", f5);

    if (f4 > f3) {
        printf("Counterintuitive: adding more memory made things worse.\n");
        printf("This is impossible under LRU, which is a 'stack algorithm.'\n");
    }
    return 0;
}
