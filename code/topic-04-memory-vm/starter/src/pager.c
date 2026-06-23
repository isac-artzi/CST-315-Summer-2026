/*
 * pager.c — CST-315 · Topic 4 · starter
 * Purpose: User-space demand-paging simulator with LRU replacement.
 *
 * STUDENT TODO: complete handle_reference() below.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FRAMES 64

typedef struct {
    int  page;        /* page currently held, or -1 if free          */
    int  last_used;   /* logical clock of the most recent reference  */
} Frame;

/* Service one page reference under LRU.
 * Returns 1 on hit, 0 on fault. *out_frame = index touched/installed. */
static int handle_reference(Frame frames[], int nframes,
                            int page, int t, int *out_frame) {
    /* ===========================================================
     * TODO Topic 4: implement LRU page replacement.
     *
     * Handle three cases in this order:
     *   1. HIT — `page` is already in some frame.
     *            Update that frame's last_used to t.
     *            Set *out_frame to that index.
     *            Return 1.
     *
     *   2. FAULT, free frame available — find the first frame with
     *      page < 0 (sentinel for "empty"). Install `page` there.
     *      Set last_used to t. Set *out_frame. Return 0.
     *
     *   3. FAULT, all frames full — find the frame with the
     *      smallest last_used (the LRU). Evict it (overwrite the
     *      page field). Install `page`. Set last_used to t.
     *      Set *out_frame. Return 0.
     * =========================================================== */

    /* TODO: replace this stub */
    (void)frames; (void)nframes; (void)page; (void)t;
    *out_frame = 0;
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
