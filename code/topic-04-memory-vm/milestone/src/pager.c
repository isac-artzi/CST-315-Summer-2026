/*
 * pager.c — CST-315 · Topic 4 · milestone
 * Purpose: User-space demand-paging simulator with LRU replacement.
 *          Reads a reference string from stdin (integers separated
 *          by whitespace). Prints a per-step trace and a final
 *          summary (hits / faults / fault rate).
 * Build:   make
 * Run:     ./pager                       # reads stdin; 4 frames default
 *          ./pager 4 < refs.txt
 *          echo "1 2 3 4 1 2 5 1 2 3 4 5" | ./pager 4
 * Notes:   POSIX. Builds on macOS and Linux.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FRAMES 64

/* One physical frame holds at most one page. -1 means free. */
typedef struct {
    int  page;        /* page currently held, or -1 if free          */
    int  last_used;   /* logical clock of the most recent reference  */
} Frame;

/* Service one page reference under LRU.
 * Returns 1 on hit, 0 on fault. Writes the affected frame index. */
static int handle_reference(Frame frames[], int nframes,
                            int page, int t, int *out_frame) {
    /* Hit? */
    for (int i = 0; i < nframes; i++) {
        if (frames[i].page == page) {
            frames[i].last_used = t;
            *out_frame = i;
            return 1;
        }
    }
    /* Fault: free frame? */
    for (int i = 0; i < nframes; i++) {
        if (frames[i].page < 0) {
            frames[i].page = page;
            frames[i].last_used = t;
            *out_frame = i;
            return 0;
        }
    }
    /* Fault: evict the LRU frame. */
    int victim = 0;
    for (int i = 1; i < nframes; i++) {
        if (frames[i].last_used < frames[victim].last_used) victim = i;
    }
    frames[victim].page = page;
    frames[victim].last_used = t;
    *out_frame = victim;
    return 0;
}

static void print_frames(Frame frames[], int n) {
    printf("[");
    for (int i = 0; i < n; i++) {
        if (frames[i].page < 0) printf(" . ");
        else                    printf("%2d ", frames[i].page);
    }
    printf("]");
}

int main(int argc, char **argv) {
    int nframes = (argc > 1) ? atoi(argv[1]) : 4;
    if (nframes < 1 || nframes > MAX_FRAMES) {
        fprintf(stderr, "pager: frame count must be 1..%d\n", MAX_FRAMES);
        return 1;
    }

    Frame frames[MAX_FRAMES];
    for (int i = 0; i < MAX_FRAMES; i++) {
        frames[i].page = -1;
        frames[i].last_used = -1;
    }

    printf("pager: %d frames, LRU replacement\n", nframes);
    printf("Type integer page references on stdin. CTRL-D to finish.\n\n");

    int hits = 0, faults = 0, t = 0;
    int page;
    while (scanf("%d", &page) == 1) {
        int which;
        int hit = handle_reference(frames, nframes, page, t, &which);
        if (hit) hits++; else faults++;
        printf("t=%2d  ref=%d  %s  frame=%d  ", t, page,
               hit ? "HIT " : "FLT ", which);
        print_frames(frames, nframes);
        printf("\n");
        t++;
    }
    int total = hits + faults;
    printf("\nReferences: %d   hits: %d   faults: %d   fault rate: %.1f%%\n",
           total, hits, faults,
           total > 0 ? 100.0 * faults / total : 0.0);
    return 0;
}
