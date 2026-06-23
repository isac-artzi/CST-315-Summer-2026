# Topic 4 · Activity 2 — Implement LRU Page Replacement in C

**Course:** CST-315 Operating Systems Lecture & Lab
**Duration:** ~40 minutes
**Format:** Individual at your laptop; pairs allowed for debugging

## Aligned objectives
2. Design, implement, and manage tables and frames.
3. Implement paging algorithms.

## What you will produce
A working LRU page replacement function in C with a test harness using the reference string `1 2 3 4 1 2 5 1 2 3 4 5`. Plus a FIFO version for comparison. This is the engine of Project 4's page-fault handler.

## Setup
- Working Linux/UNIX environment.
- A new directory: `~/CST-315/Topic4`.

## Steps

### Part A — Function signature (5 min)

We'll write:

```c
int handle_reference(int *frames, int *last_used, int n_frames, int page, int t);
```

Returns 1 on hit, 0 on fault. Updates `frames` and `last_used` in place.

### Part B — Implement (25 min)

Create `~/CST-315/Topic4/lru.c`:

```c
/* lru.c — LRU page replacement */
#include <stdio.h>
#include <stdlib.h>

#define N_FRAMES 4

/* returns 1 if hit, 0 if fault */
int handle_reference(int *frames, int *last_used, int n_frames, int page, int t) {
    /* Check for hit */
    for (int i = 0; i < n_frames; i++) {
        if (frames[i] == page) {
            last_used[i] = t;
            return 1;
        }
    }
    /* Fault: find empty frame */
    for (int i = 0; i < n_frames; i++) {
        if (frames[i] == -1) {
            frames[i] = page;
            last_used[i] = t;
            return 0;
        }
    }
    /* No empty frame: evict LRU */
    int victim = 0;
    for (int i = 1; i < n_frames; i++) {
        if (last_used[i] < last_used[victim]) victim = i;
    }
    frames[victim] = page;
    last_used[victim] = t;
    return 0;
}

void print_frames(int *frames, int n) {
    printf("[");
    for (int i = 0; i < n; i++) {
        if (frames[i] == -1) printf(" . ");
        else printf("%2d ", frames[i]);
    }
    printf("]");
}

int main(int argc, char **argv) {
    int frames[N_FRAMES];
    int last_used[N_FRAMES];
    for (int i = 0; i < N_FRAMES; i++) { frames[i] = -1; last_used[i] = -1; }

    int refs[] = {1,2,3,4,1,2,5,1,2,3,4,5};
    int n_refs = sizeof(refs)/sizeof(refs[0]);
    int hits = 0, faults = 0;

    for (int t = 0; t < n_refs; t++) {
        int hit = handle_reference(frames, last_used, N_FRAMES, refs[t], t);
        if (hit) hits++; else faults++;
        printf("t=%2d  ref=%d  %s   ", t, refs[t], hit ? "HIT " : "FLT ");
        print_frames(frames, N_FRAMES);
        printf("\n");
    }
    printf("\nHits: %d  Faults: %d  Fault rate: %.0f%%\n",
           hits, faults, 100.0 * faults / n_refs);
    return 0;
}
```

Compile and run:

```bash
gcc lru.c -o lru
./lru
```

Expected: 4 hits, 8 faults (67% fault rate). Screenshot.

### Part C — Compare to FIFO (10 min)

Copy `lru.c` to `fifo.c`. Replace the LRU eviction block with FIFO logic: track each frame's load time and evict the oldest-loaded frame.

Run both. FIFO produces 10 faults on the same reference string. Screenshot the difference.

Commit both files to `~/CST-315/Topic4/` and push.

## Discussion prompts
- LRU needs to track `last_used` for every frame. What is the cost of doing this exactly in hardware?
- The Clock algorithm in your reading approximates LRU using a single reference bit. Sketch its data structure.
- Project 4 requires demand paging: pages are only brought in on first reference. How does that simplify your code's initialization?

## Stretch goal
Demonstrate **Belady's anomaly**: run your FIFO implementation with 3 frames and then with 4 frames on the reference string `1 2 3 4 1 2 5 1 2 3 4 5`. With 3 frames you'll get 9 faults; with 4 frames, 10. Then verify LRU's fault count drops or stays the same as frames increase. The anomaly is one of the classic surprises in OS theory.

---
**Companion lesson:** [Topic_4_Memory_Virtual_Memory.html](../lessons/Topic_4_Memory_Virtual_Memory.html)
**Master visualization:** [Factory_OS_Master.html](../Factory_OS_Master.html)
