# Topic 4 · Activity 1 — Hand-Walk a 12-Bit Virtual Address through Paging

**Course:** CST-315 Operating Systems Lecture & Lab
**Duration:** ~20 minutes
**Format:** Individual on paper

## Aligned objectives
1. Manage memory resources to extend the memory available for processes to run.
2. Design, implement, and manage tables and frames.

## What you will produce
Six worked-out address translations on paper, with explicit page numbers, offsets, frame lookups, and fault notes.

## Setup
- Pencil and paper.
- Address scheme: **12-bit virtual address space, 256-byte pages.**
- Page table snapshot:

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

## Steps

1. **Frame (5 min).** Watch the instructor walk the example `0xA37`. Confirm: 256-byte pages → offset = 8 bits → page number = top 4 bits. So `0xA37` is page `0xA`, offset `0x37`, which the table maps to frame `0x5` → physical `0x537`.

2. **Translate (12 min).** For each address below, write:
   - the page number
   - the offset
   - whether the access faults
   - the physical address (or "FAULT — call the runner")

   Addresses:
   - 0x305
   - 0x812
   - 0x1A0
   - 0xBFF
   - 0x4C0
   - 0x980

3. **Debrief (3 min).** Compare answers across the room. Discuss what the OS does on each fault.

## Discussion prompts
- Two of these addresses fault. Which pages do they target?
- If you had to allocate frames after these accesses, which page-replacement choice would you make for each fault?
- The snapshot's "present? no" entries don't have a frame number. Where do those pages live?

## Stretch goal
Compute the size of this 12-entry page table at 2 bytes per entry. Then compute the same for a 32-bit virtual address with 4 KB pages and a flat page table. Compare. The contrast is why multi-level page tables exist.

---
**Companion lesson:** [Topic_4_Memory_Virtual_Memory.html](../lessons/Topic_4_Memory_Virtual_Memory.html)
**Master visualization:** [Factory_OS_Master.html](../Factory_OS_Master.html)
