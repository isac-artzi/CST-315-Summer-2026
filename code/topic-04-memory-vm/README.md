# Topic 4 — Memory Management and Virtual Memory

Three subprojects:

- **`milestone/`** — `pager`: a demand-paging simulator with LRU page replacement. Reads a reference string from stdin or a file. Prints per-step trace and final fault rate.
- **`starter/`** — same shape with TODOs on the page-table struct, fault handler, and LRU eviction.
- **`demos/`** — INSTRUCTOR ONLY. `mmap_demo`, `fault_timer`, `bump_allocator`, `belady_anomaly`.

The cumulative shell from Topic 3 is unchanged this week; this topic adds a standalone simulator binary that the shell can invoke (`lopeShell> ./pager ref.txt`).

## Aligned course materials

- Lesson: `lessons/Topic_4_Memory_Virtual_Memory.html`
- Activity 1: `activities/Topic_4_Activity_1.md`
- Activity 2: `activities/Topic_4_Activity_2.md`
- Project: Project 4 (Pager / Virtual Memory Manager)
