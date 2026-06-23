# PRD: CST-315 Interactive Course Materials

**Owner:** Isac (instructor)
**Status:** Draft for review
**Scope:** 8 interactive lessons + 8 activity documents + 1 master OS visualization, for the 7-week Summer session of CST-315.

---

## 1. Objectives

Produce a coherent, reusable set of classroom materials that:

1. Let the instructor lecture against a live, interactive HTML page per topic, with simulations and diagrams that illustrate OS concepts in real time.
2. Provide two in-class activity sets per topic that bridge from lecture concepts to the graded assignments due in that topic.
3. Anchor every topic in a single shared mental model — a **factory / production-line metaphor for the OS** — visualized once and referenced throughout the course.

## 2. The Core Metaphor: The OS as a Factory

The operating system is presented as a factory that turns raw program instructions into finished computational output. Every OS concept maps to a concrete factory element. This mapping is fixed and used identically in every lesson, activity, and diagram.

| OS Concept | Factory Element |
|---|---|
| CPU core | Assembly-line station (worker) |
| Process | Work order moving through the factory |
| Thread | A task on a work order that can be done in parallel |
| Process Control Block (PCB) | Clipboard attached to each work order |
| Scheduler | Floor supervisor assigning work orders to stations |
| Ready queue | Inbound staging area |
| Wait/blocked queue | "Waiting on parts" holding area |
| Context switch | Worker swapping clipboards between work orders |
| RAM / main memory | Staging racks next to the line |
| Virtual memory | Off-site warehouse |
| Page table | Warehouse index / shelf map |
| Page fault | Worker asks for parts not on the rack; runner fetches from the warehouse |
| Swap space | Overflow warehouse |
| Semaphore | Signal light at a shared tool station |
| Monitor | Locked tool room with a single key and a queue |
| Deadlock | Two workers each holding a tool the other needs |
| File system | Document archive / records room |
| Inode / file descriptor | File folder tab + checkout slip |
| Directory tree | Filing cabinet with nested drawers |
| I/O device | Loading dock |
| Interrupt | Intercom alert that pauses the line |
| Shell | Front-desk dispatcher taking job tickets |
| System call | Worker filling out an official request form to management |
| Kernel | Plant manager's office (only path to restricted resources) |
| User mode / kernel mode | Shop floor vs. manager's office |
| Virtual machine | A fully contained mini-factory inside the factory |
| Hypervisor | Building manager allocating space to multiple mini-factories |
| Container | A modular workstation kit shared across mini-factories |
| Malware / injection | Sabotage: forged paperwork that alters a work order |

This table is the canonical reference. Every lesson opens by pointing at the relevant slice of the factory.

## 3. The Master Visualization

**Format:** Single self-contained `Factory_OS_Master.html` file.

**Purpose:** A reference diagram of the entire OS-as-factory, displayed at the start of each lesson, with the current topic highlighted.

**Required elements:**

- A top-down factory floor plan rendered in SVG (D3 for layout, plain SVG for graphics).
- Distinct zones: **Front Desk (shell)**, **Floor (CPU + scheduler)**, **Staging Racks (memory)**, **Warehouse (virtual memory + swap)**, **Records Room (file system)**, **Loading Dock (I/O)**, **Manager's Office (kernel)**, **Mini-Factories (VMs)**.
- Animated flow arrows showing a work order's life cycle: dispatcher → ready queue → station → wait queue → completion.
- A **topic spotlight** mechanism: each lesson's HTML calls `Factory.highlight("scheduler")` (or similar) to dim the rest of the floor and emphasize the current zone.
- Hover tooltips that show the OS-term equivalent for each factory element.
- A small legend in the corner with the metaphor table (collapsible).

**Reuse pattern:** Each lesson `<iframe>`s or inlines a stripped version of the master visualization at the top of the page, with the appropriate zone highlighted.

## 4. Lesson Template

**One file per topic:** `Topic_N_<short_name>.html`, fully self-contained except for CDN imports.

**Page structure (top to bottom):**

1. **Header strip** — Topic number, title, learning objectives (verbatim from syllabus), Stallings chapter references.
2. **Factory context panel** — Embedded slice of the master visualization with the relevant zone highlighted. One sentence explaining where this topic fits in the factory.
3. **Concept sections (3–5 per lesson)** — Each section contains:
   - A 2–4 sentence concept summary in plain prose.
   - A diagram (SVG or Mermaid) showing the concept structurally.
   - One interactive simulation the instructor can drive live (see §5).
   - A "factory translation" callout: how this concept manifests in the metaphor.
4. **Check-for-understanding** — 3–5 inline multiple-choice or short-answer prompts the instructor can use as cold-call material. No grading, just hooks.
5. **Bridge to assignments** — Short section naming the topic's quiz, projects, and assignments, with one sentence each on how the lecture concepts map to deliverables.
6. **Footer** — Links to the topic's activity document and to the master visualization.

**Pacing target:** ~45–60 minutes of lecture material per page. The instructor drives narrative; the page is the visual aid.

**Visual style:** Light theme, factory-blueprint aesthetic — off-white background (`#f5f3ee`), navy ink (`#1b2a4e`) for primary text and lines, a single accent color per topic (defined in §7), monospace for code, sans-serif for prose (Inter or system stack). Subtle paper-grain texture optional.

## 5. Simulation Library

A small set of reusable interactive widgets, instantiated with different parameters per lesson:

| Widget | Built with | Used in topics |
|---|---|---|
| `ProcessLifecycleSim` — process state machine with draggable processes | D3 | 2, 3, 5 |
| `SchedulerSim` — visual Gantt chart with FCFS / RR / SRT / priority toggles | D3 + Chart.js | 5 |
| `ConcurrencySim` — two threads sharing a buffer; semaphore on/off | p5.js | 2, 3 |
| `DeadlockSim` — resource allocation graph with cycle detection | D3 | 3 |
| `PagingSim` — virtual addresses → page table → physical frames, with faults | p5.js | 4 |
| `FileSystemTree` — interactive directory tree with CRUD | D3 | 6 |
| `ShellSim` — fake terminal that parses commands and shows fork/exec flow | vanilla JS | 1, 2, 3, 6 |
| `InjectionSim` — show command rewriting in flight | vanilla JS | 7 |
| `VirtualizationSim` — type-1 vs type-2 hypervisor stack | Mermaid + SVG | 8 |

Each widget is a single `<script>` block exposing a constructor that takes a target `<div>` ID and a config object. Widgets are inlined into each lesson page — no shared JS file, so each lesson remains a single portable HTML.

## 6. Activity Document Template

**One file per topic:** `Topic_N_Activities.md`. Markdown for now; can be rendered to HTML or DOCX later. Each document contains **two in-class activity sets**.

**Per-activity structure:**

- **Title and duration estimate**
- **Aligned objectives** (verbatim from syllabus)
- **Bridge to assignment** — one sentence on which graded deliverable this prepares students for
- **Setup** — what students need open, what files to clone, etc.
- **Steps** — numbered, imperative
- **Discussion prompts** — 2–3 questions to debrief
- **Instructor notes** — common pitfalls, expected outcomes, time checkpoints
- **Stretch goal** — for fast finishers

Activity 1 of each topic is **conceptual / paper-and-whiteboard** style (warm-up, sketch a diagram, trace by hand). Activity 2 is **hands-on coding** that directly produces a fragment usable in the topic's project or assignment.

## 7. Per-Topic Content Matrix

Each row defines the lesson and activities for one topic. Activities are aligned with assignments shown in the rightmost column.

### Topic 1 — Overview of Computer Systems and OS
- **Accent color:** `#c9742e` (orange)
- **Factory focus:** Whole-factory tour
- **Concept sections:** Hardware stack; OS roles; the shell; UNIX/Linux command surface; compile-and-run lifecycle
- **Simulations:** `ShellSim` (basic), `ProcessLifecycleSim` (intro view)
- **Activity 1 (concept):** Sketch your computer as a factory — students annotate a blank floor plan with OS components
- **Activity 2 (hands-on):** Linux setup smoke test — students compile `HelloWorld.c`, initialize the GitHub repo, push first commit
- **Bridges to:** Project 1 (UNIX familiarity), Topic 1 Quiz, Lab Questions 1–4

### Topic 2 — Processes and Threads
- **Accent color:** `#2e7d6b` (teal)
- **Factory focus:** Floor + clipboards (PCBs)
- **Concept sections:** Process vs program; PCB internals; state model; threads; fork/exec/wait
- **Simulations:** `ProcessLifecycleSim`, `ShellSim` (fork visualization), `ConcurrencySim`
- **Activity 1 (concept):** Trace a `fork()`+`exec()` sequence on the factory diagram, identify clipboard handoffs
- **Activity 2 (hands-on):** Implement and instrument a minimal CLI loop that forks a child for one command, observing PID/PPID
- **Bridges to:** Project 2 (Command Line Interpreter), CLC Assignment 1 (Producer/Consumer)

### Topic 3 — Mutual Exclusion and Concurrency
- **Accent color:** `#a83232` (red)
- **Factory focus:** Shared tool station + signal lights
- **Concept sections:** Critical sections; semaphores; monitors; classical problems (producer/consumer, readers/writers, dining philosophers); deadlock conditions
- **Simulations:** `ConcurrencySim` (with sem on/off), `DeadlockSim`
- **Activity 1 (concept):** Diagnose a deadlock from a resource allocation graph; propose two fixes
- **Activity 2 (hands-on):** Convert the producer/consumer from busy-wait to semaphore-protected and measure CPU time
- **Bridges to:** Project 3 (Improved CLI), Assignment 2 (Monitors & Semaphores), Assignment 3 (Deadlock Avoidance)

### Topic 4 — Memory Management and Virtual Memory
- **Accent color:** `#3b5fa8` (blue)
- **Factory focus:** Staging racks + warehouse + runner
- **Concept sections:** Address spaces; paging; page tables; TLB; demand paging; replacement algorithms (FIFO, LRU, clock)
- **Simulations:** `PagingSim` (configurable page count, fault display)
- **Activity 1 (concept):** Walk through page table lookups by hand for a 12-bit address; predict faults
- **Activity 2 (hands-on):** Implement a page replacement algorithm (LRU) in C, run a given reference string, compute fault rate
- **Bridges to:** Project 4 (Pager / VMM), Topic 4 Quiz, Lab Questions on memory

### Topic 5 — Processor Scheduling
- **Accent color:** `#7a4a9e` (purple)
- **Factory focus:** Floor supervisor + inbound queue
- **Concept sections:** Scheduling criteria; FCFS, SJF, SRTF, RR, priority, MLFQ; Linux CFS overview (modern concept)
- **Simulations:** `SchedulerSim` with algorithm toggle and metrics readout
- **Activity 1 (concept):** Given a process set, hand-draw Gantt charts for 3 algorithms; compute avg wait/turnaround
- **Activity 2 (hands-on):** Implement RR scheduling decision logic in C; integrate with shell's process list
- **Bridges to:** Project 5 (Short-Term Process Scheduler), Topics 5–6 Quiz

### Topic 6 — I/O and File Management
- **Accent color:** `#4a8a3d` (green)
- **Factory focus:** Records room + loading dock
- **Concept sections:** I/O models; buffering; file abstraction; inodes/descriptors; allocation methods; RAID overview
- **Simulations:** `FileSystemTree` (CRUD operations live), `ShellSim` extended with `mkdir`/`ls`/`rm`
- **Activity 1 (concept):** Design the data structures for a directory entry; whiteboard the trade-offs of contiguous vs linked vs indexed allocation
- **Activity 2 (hands-on):** Implement `mkdir` and `rmdir` (with non-empty handling) in the shell project's file manager
- **Bridges to:** Project 6 (File System Manager)

### Topic 7 — Embedded Systems and Security
- **Accent color:** `#b8862b` (amber)
- **Factory focus:** Mini-factories + sabotage scenarios
- **Concept sections:** Embedded constraints; trust boundaries; command injection; modern OS security (sandboxing, SELinux/AppArmor, secure boot) — modern concept supplements here
- **Simulations:** `InjectionSim` showing command rewriting in flight
- **Activity 1 (concept):** Read a vulnerable C snippet aloud, identify the injection vector, propose a sanitization strategy
- **Activity 2 (hands-on):** Write a scanner that flags files containing suspicious `system()` / `exec*()` patterns
- **Bridges to:** Assignment 4 (Injection Virus), Topics 7–8 Quiz

### Topic 8 — Virtual Environments
- **Accent color:** `#2f8a8a` (cyan)
- **Factory focus:** Building manager + multiple mini-factories
- **Concept sections:** Type-1 vs type-2 hypervisors; full vs para-virtualization; containers vs VMs; modern concept supplement: Docker, Kubernetes, Firecracker, confidential computing
- **Simulations:** `VirtualizationSim` showing stack composition under each model
- **Activity 1 (concept):** Architect a virtualization solution for a stated workload; defend the choice against three criteria
- **Activity 2 (hands-on):** Build a container image for the shell project and run it; compare boot time to a VM run
- **Bridges to:** Assignment 5 (Assessing Virtualization Software), Project 7 (Shell Presentation)

## 8. Tech Stack

- HTML5, modern ES module-free JS (so files open via `file://`).
- CDN libraries: **D3 v7**, **p5.js 1.9**, **Mermaid 10**, **Chart.js 4**. All pinned, all from `cdn.jsdelivr.net`.
- No build step. No npm. Open in any browser; works offline once cached.
- Single-file artifacts everywhere.

## 9. File and Folder Layout

```
/CST-315
├── Welcome_to_CST-315.md
├── PRD_Course_Materials.md            ← this file
├── Factory_OS_Master.html             ← master visualization
├── lessons/
│   ├── Topic_1_Overview.html
│   ├── Topic_2_Processes_Threads.html
│   ├── ...
│   └── Topic_8_Virtual_Environments.html
└── activities/
    ├── Topic_1_Activities.md
    ├── ...
    └── Topic_8_Activities.md
```

## 10. Production Order

1. **Master visualization** — done first; everything else depends on it.
2. **Topic 1 lesson + activities** — full vertical slice to validate the templates end-to-end.
3. **Review pause** — instructor reviews the slice; templates revised if needed.
4. **Topics 2–8** — produced in syllabus order, two artifacts per topic (lesson + activities).
5. **Final cross-link pass** — every lesson links to its activity doc and to the master; activity docs link back.

## 11. Open Questions for Instructor Approval

1. **Modern concept depth.** §7 lists modern-concept supplements at Topics 5, 7, and 8. Is that the right placement, or should modern callouts appear in every topic?
2. **Activity duration.** Should activities target ~20 min each (so both fit in one class block alongside lecture), or ~45 min each (one per class period)?
3. **Faith integration.** Topic 8 has a required Christian-principles discussion in Assignment 5. Should the Topic 8 lesson scaffold this, or stay purely technical and leave faith integration to the assignment?
4. **Student device assumption.** Are students bringing laptops to every class (so hands-on activities run on their own machines), or do some sessions need a non-coding fallback?
5. **Assessment alignment.** Should activity docs include suggested rubrics, or stay ungraded scaffolding?

## 12. Acceptance Criteria for the Full Build

A topic is "done" when:

- [ ] Lesson HTML opens cleanly in Chrome and Firefox from disk.
- [ ] Master visualization slice loads at the top with the correct zone highlighted.
- [ ] Every syllabus objective for the topic is addressed by a labeled concept section.
- [ ] At least one interactive simulation runs without errors.
- [ ] Activity doc lists two activities, each with all template fields populated.
- [ ] Both activities trace a clear path to the topic's graded deliverable.
- [ ] Lesson links to the activity doc; activity doc links to the lesson and to the master.
