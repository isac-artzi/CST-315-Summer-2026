# Topic 4 — In-Class Activities

**Course:** CST-315 Operating Systems Lecture & Lab
**Topic:** Memory Management and Virtual Memory
**Reading:** Stallings, Chapters 7–8
**Companion lesson:** [Topic_4_Memory_Virtual_Memory.html](../lessons/Topic_4_Memory_Virtual_Memory.html)
**Master visualization:** [Factory_OS_Master.html](../Factory_OS_Master.html)

Activity 1 makes paging concrete with hand-calculated address translation and a small page replacement trace. Activity 2 has students implement an LRU page replacement function in C — the engine of Project 4.

---

## Activity 1 — Hand-walk a 12-bit virtual address through paging

**Duration:** ~20 minutes (5 min framing · 12 min work · 3 min debrief)
**Format:** Individual on paper
**Aligned objectives:** 1, 2

### Bridge to assignment
Project 4 asks students to design the page table data structures and the fault-handling algorithm. Doing the translation by hand fixes the bit-arithmetic in their minds so the code feels obvious instead of mysterious.

### Setup
- Pencil and paper. No laptop needed.
- Hand out the address scheme: **12-bit virtual address space, 256-byte pages.**
- Provide the following page table snapshot:

  | virtual page | physical frame | present? |
  |---:|---:|:---:|
  | 0x0 | 0x4 | yes |
  | 0x1 | — | no |
  | 0x2 | 0x1 | yes |
  | 0x3 | 0x6 | yes |
  | 0x4 | — | no |
  | 0x5 | 0x0 | yes |
  | 0x6 | 0x2 | yes |
  | 0x7 | — | no |
  | 0x8 | 0x3 | yes |
  | 0x9 | — | no |
  | 0xA | 0x5 | yes |
  | 0xB | 0x7 | yes |

### Steps
1. **Frame (5 min).** Project the lesson's worked example (`0xA37 → 0x537`). Confirm offset bits = 8, page-number bits = 4.

2. **Translate (12 min).** For each of the following virtual addresses, write down: (a) the page number, (b) the offset, (c) whether the access faults, (d) the physical address (or "FAULT — call the runner").

   - 0x305
   - 0x812
   - 0x1A0
   - 0xBFF
   - 0x4C0
   - 0x980

3. **Debrief (3 min).** Compare answers around the room. Address the fault cases: what does the OS do?

### Discussion prompts
- Two of these addresses fault. Which pages do they target? Are any of them likely candidates to be brought in (e.g., next to a frequently-touched page)?
- If you had to allocate frames after these accesses, which page-replacement choice would you make for each fault?
- The page-table entries in the snapshot use 4 bits for the frame number, which would imply 16 frames total. We only see 12 entries — what does "present? no" mean about disk vs RAM?

### Instructor notes
- Common pitfall: students confuse bits with hex digits. Emphasize: each hex digit = 4 bits, so 12-bit address = 3 hex digits.
- Expected outcomes: 0x305 → page 3, offset 0x05, frame 6 → 0x605. 0x812 → page 8, offset 0x12 → 0x312. 0x1A0 → page 1, offset 0xA0 → FAULT. 0xBFF → page B, offset 0xFF → 0x7FF. 0x4C0 → page 4 → FAULT. 0x980 → page 9 → FAULT.
- Time checkpoint: by minute 10 every student should have completed at least four translations.

### Stretch goal
Compute the size in bytes of this 12-entry page table if each entry uses 2 bytes (one byte for frame number + bits, one for control). Now compute the same for a 32-bit virtual address with 4 KB pages and a flat page table. The point: multi-level page tables are not optional.

---

## Activity 2 — Implement LRU page replacement in C

**Duration:** ~40 minutes (5 min frame · 25 min code · 10 min test and discuss)
**Format:** Individual at laptop; pairs allowed for debugging
**Aligned objectives:** 2, 3

### Bridge to assignment
This activity produces a working LRU replacement function in C with a test harness. Drop it into Project 4 as the engine of your page-fault handler. The data structures (frame array, last-used timestamps) match Project 4's requirements.

### Setup
- Working Linux/UNIX environment.
- A new directory: `~/CST-315/Topic4`.
- A reference string to test against: `1 2 3 4 1 2 5 1 2 3 4 5` — chosen because LRU and FIFO produce different fault counts.

### Steps

#### Part A — Frame the function (5 min)
We'll write a function with this signature:

```c
int handle_reference(int *frames, int *last_used, int n_frames, int page, int t);
```

Returns 1 if it was a hit, 0 if a fault. Updates `frames` and `last_used` in place.

#### Part B — Implement (25 min)

1. Create `~/CST-315/Topic4/lru.c`:

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

2. Compile and run:

```bash
gcc lru.c -o lru
./lru
```

3. Verify: 4 hits, 8 faults (fault rate 67%).

#### Part C — Compare to FIFO (10 min)
4. Copy `lru.c` to `fifo.c`. Replace the "find LRU victim" block with FIFO logic — track each frame's load time and evict the oldest-loaded.

5. Run both. The same reference string with FIFO produces 10 faults (rate 83%) — worse than LRU.

6. Commit both files to `~/CST-315/Topic4/` and push.

### Discussion prompts
- LRU needs to track `last_used` for every frame. What is the cost of doing this exactly in hardware?
- The Clock algorithm (in your reading) approximates LRU using a single reference bit. Sketch its data structure.
- Project 4 requires demand paging: pages are only brought in on first reference. How does that simplify the initialization step in your code?

### Instructor notes
- Common pitfall: initializing `last_used` to 0 instead of -1 — then on the first reference, an empty frame may look "used at time 0" and tie with a fresh access. The -1 sentinel avoids this.
- Expected outcomes: every student should produce both binaries and observe LRU outperforming FIFO. Some will try the reference string `1 2 3 4 1 2 5 1 2 3 4 5` with 3 frames — that's Belady's anomaly territory; with FIFO you'll see more faults with 4 frames than with 3.
- Time checkpoint: by minute 30 every student should have LRU producing the right fault count.

### Stretch goal
Demonstrate Belady's anomaly. Use the reference string `1 2 3 4 1 2 5 1 2 3 4 5` with **3** frames and then **4** frames using your FIFO implementation. Report the fault counts. Hint: FIFO with 3 frames produces 9 faults; with 4 frames it produces 10. Then run the same on LRU — both 3-frame and 4-frame counts are monotonic. This is the classic FIFO pathology.

---

## Closeout (last 2 minutes of class)

Direct students to:
- Begin sketching the page-table data structures for Project 4 tonight. Two arrays (`frames[N]` and `last_used[N]`) extend naturally to a struct array per process.
- Read Stallings Ch. 8 on virtual memory policy before next class. Topic 5 covers scheduling — closely related to page management when faults trigger blocks.
- If they finished early, try the same reference string with 3 frames to see how memory pressure changes the fault rate.
