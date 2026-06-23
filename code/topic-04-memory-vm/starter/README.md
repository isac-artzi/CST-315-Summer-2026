# Topic 4 · Starter — `pager`

A skeleton demand-paging simulator. The harness (I/O, framework, summary printing) is in place. You implement the page-reference handler with LRU eviction.

## What you have to do

Open `src/pager.c`. One TODO is marked: complete `handle_reference()`.

You must handle three cases:
1. **Hit** — the page is already in some frame; just update `last_used`.
2. **Fault, free frame** — install the page in the first free frame.
3. **Fault, full** — find the LRU frame and evict it, then install.

## Build, run, test
```bash
make
echo "1 2 3 4 1 2 5 1 2 3 4 5" | ./pager 4
make test
```

The canonical 12-reference sequence with 4 frames should produce 4 hits and 8 faults under LRU.

## Hints
- `frames[i].page == -1` means free.
- `frames[i].last_used` is the logical clock from the most recent reference to that page.
- LRU victim = the frame whose `last_used` is smallest.

## Bridges to
- Lesson: `lessons/Topic_4_Memory_Virtual_Memory.html`
- Project 4 — Pager (Virtual Memory Manager)
