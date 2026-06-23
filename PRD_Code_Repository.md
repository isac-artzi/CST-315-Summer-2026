# PRD: CST-315 Code Repository

**Owner:** Isac (instructor)
**Status:** Draft for review
**Scope:** One local git repo with subfolders per topic, three mini-projects per topic (milestone implementation, student starter, instructor demos), plus top-level scaffolding.

---

## 1. Goals

Produce a single coherent codebase that:

1. Gives students a working, incremental reference implementation of the UNIX/Linux shell project — one milestone per topic, building toward the complete shell by Topic 8.
2. Gives students a starter skeleton per topic with TODO comments scoped to the current milestone's new work, so they can begin coding from a known-good baseline.
3. Gives the instructor demo programs for the additional concepts covered in each topic — programs not shared with students, used for in-class examples.
4. Builds cleanly on Linux/UNIX with `gcc` and `make`, runs on the development environment expected by the syllabus, and exposes a uniform README/Makefile pattern across every subproject.
5. Is `git init`-ed so it can be pushed to GitHub (private) and shared with students by directory paths, not by leaking instructor demos.

## 2. Repo Structure

```
CST-315-Code/
├── README.md                          ← top-level orientation
├── .gitignore                         ← excludes build artifacts, *.o, binaries
├── LICENSE                            ← course-use note
├── common/                            ← shared headers used across milestones
│   ├── README.md
│   └── shell_types.h                  ← Process struct, State enum, queue ops
├── topic-01-overview/
│   ├── README.md                      ← topic-level orientation
│   ├── milestone/                     ← #1: reference implementation
│   │   ├── README.md
│   │   ├── Makefile
│   │   ├── src/                       ← C source
│   │   └── test.sh                    ← smoke test
│   ├── starter/                       ← #2: student skeleton (TODOs)
│   │   ├── README.md
│   │   ├── Makefile
│   │   └── src/                       ← C source with TODOs
│   └── demos/                         ← #3: instructor only
│       ├── README.md                  ← NOT-FOR-STUDENTS warning
│       ├── Makefile
│       └── src/                       ← multiple short demo programs
├── topic-02-processes-threads/
│   └── ... (same pattern)
├── ... (topics 3-8)
└── reference-shell/                   ← symlink or copy of topic-08/milestone/
```

A folder-per-topic with a folder-per-mini-project gives each binary a clean build with one `make` from anywhere. No nested Makefiles, no recursive builds. Students cd into `topic-XX-foo/starter/` and `make`.

## 3. Cumulative vs Isolated Skeletons — My Recommendation

You asked which approach is better. **I recommend a hybrid: cumulative starters by default, plus the means to drop in a reference implementation of any prior milestone.**

### The three options

- **Isolated starters.** Each topic's skeleton is a clean slate. Pro: easy to grade in isolation; a single bad week doesn't compound. Con: doesn't mirror how the project actually grows — Project 5 truly does require a working command interpreter from Project 2. Students who do isolated work end up writing throwaway code.

- **Pure cumulative starters.** Each topic's skeleton is the prior topic's reference + new TODOs. Pro: matches the project's architecture exactly. Con: a student who fell behind on Project 3 cannot start Project 4 from clean ground.

- **Hybrid (recommended).** Each topic's starter is cumulative — it contains the *reference* implementation of every prior milestone, plus TODO skeletons for the current milestone's new work. Students who did their own prior work can replace specific files with their own. Students who fell behind get a working baseline and only have to focus on the current milestone.

### Why the hybrid wins for this course

Three reasons specific to your 7-week condensed session:

1. **The compressed schedule punishes single-week setbacks.** Pure cumulative kills students who get sick in Week 3. Isolated kills the project's architectural value. Hybrid makes failure recoverable.
2. **It mirrors industry practice.** Every engineer who has ever joined a project mid-stream got a working codebase plus tickets for the current sprint. Modeling that habit is part of the value of the course.
3. **It separates "did the work" from "understood the work."** A student who uses the prior-week reference is signaling "I want to move on" — fine, as long as they still demonstrate understanding through the current milestone. The quiz and lab questions cover understanding; the milestone covers implementation. This separates the assessment cleanly.

### How it shows up in practice

Each `topic-N/starter/src/` contains:
- **Files with completed prior code** clearly marked: `// === Reference from Topic K. You may replace with your own version. ===`
- **Files with current TODOs** marked: `// === TODO Topic N: implement <thing>. See README §N. ===`

Students replace whichever files they want with their own work; they implement the TODOs; they submit the result.

## 4. Per-Topic Content Matrix

Each row describes one topic's three mini-projects. The milestone is the cumulative shell; the starter mirrors it with TODOs on the current new work; the demos illustrate other Topic concepts.

### Topic 1 — Overview
- **Milestone:** `HelloWorld.c` + `test.sh` + Git/GitHub setup notes. Aligns with Project 1.
- **Starter:** Empty `HelloWorld.c` with TODO header; `test.sh` with TODO commands list.
- **Demos:** `pipe_demo.c` (pipes + dup2), `env_demo.c` (getenv/setenv), `procfs_walker.c` (read /proc), `whoami_clone.c` (getuid + getpwuid).

### Topic 2 — Processes and Threads
- **Milestone:** Minimal forking shell `lopeShell` — interactive mode, single command per line, fork+exec+wait loop, `exit` builtin. (= Project 2 baseline.)
- **Starter:** Cumulative; TODOs cover the fork/exec/wait body, tokenizer, and exit handling.
- **Demos:** `pthread_basics.c` (create + join), `zombie_demo.c` (parent forgets to wait), `signal_demo.c` (SIGINT + SIGCHLD handlers), `prodcons_busywait.c` (the bad version, for contrast with Topic 3).

### Topic 3 — Mutual Exclusion and Concurrency
- **Milestone:** Shell extended to handle semicolon-separated multi-command lines concurrently — fork-all-then-wait-all pattern; CTRL-X exit shortcut; at least one new built-in (`cd`). (= Project 3 baseline.)
- **Starter:** Cumulative; TODOs cover the multi-fork dispatch, child tracking array, signal handling for CTRL-X.
- **Demos:** `prodcons_safe.c` (semaphores), `prodcons_monitor.c` (pthread mutex + cond), `deadlock_demo.c` (two threads, two mutexes, opposite order), `dining_phil.c` (four-philosopher version, deadlock + fix).

### Topic 4 — Memory Management and Virtual Memory
- **Milestone:** Pager subsystem — a separate process or module the shell invokes; demand paging with a configurable frame count and LRU replacement; reports fault rate. (= Project 4 baseline.)
- **Starter:** Cumulative; TODOs cover the page-table struct, fault handler, LRU eviction.
- **Demos:** `mmap_demo.c` (memory-map a file, see page-cache behavior), `fault_timer.c` (rdtsc-based fault cost measurement), `bump_allocator.c` (simple custom allocator), `belady_anomaly.c` (FIFO with 3 vs 4 frames).

### Topic 5 — Processor Scheduling
- **Milestone:** Scheduler module — process queue, RR scheduling (configurable quantum), `procs` / `procs -a` / `procs -a -si` commands integrated into the shell; priority modification command. (= Project 5 baseline.)
- **Starter:** Cumulative; TODOs cover the queue ops, scheduler decision function, procs formatting, priority command.
- **Demos:** `fcfs_sim.c`, `sjf_sim.c`, `mlfq_sim.c`, `realtime_demo.c` (uses `sched_setscheduler` to compare CFS vs SCHED_FIFO behavior — Linux only; macOS notes included).

### Topic 6 — I/O and File Management
- **Milestone:** In-memory file system — hierarchical tree, all 13 Project 6 commands (`mkdir`, `rmdir`, `touch`, `rm`, `mv`, `cp`, `cp -r`, `tree`, `find`, `info`, `info -d`, `dirinfo`, `dirinfo -d`). (= Project 6 baseline.)
- **Starter:** Cumulative; TODOs cover path parsing, recursive operations, find traversal.
- **Demos:** `stat_demo.c` (inspect inodes), `fd_table_demo.c` (open the same file twice, observe offsets), `simple_raid0.c` (stripe across two files), `mmap_io.c` (vs read/write).

### Topic 7 — Embedded Systems and Security
- **Milestone:** Shell hardening pass — filename sanitization, argv-quoting verification, exec-path checks. Also Assignment 4 standalone deliverables: the injection virus (`virus.c`) and detector (`scan.c`). (= Assignment 4 + a small Project 6/7 hardening commit.)
- **Starter:** Cumulative; TODOs cover the sanitizer, virus payload, scanner walking.
- **Demos:** `aslr_demo.c` (run twice, observe addresses), `stack_canary_demo.c` (compile with and without `-fstack-protector`), `seccomp_demo.c` (Linux only; macOS notes), `setuid_demo.c` (capabilities concept).

### Topic 8 — Virtual Environments
- **Milestone:** Final cumulative shell, plus `Dockerfile` to build a container image of it. Project 7 presentation deliverables (presentation source, screencast script). (= Project 7 baseline.)
- **Starter:** No new TODOs in the shell itself; the work here is integration, documentation, and the Dockerfile.
- **Demos:** `namespace_demo.c` (Linux only; macOS notes), `cgroup_demo.sh` (Linux only), `from_scratch_container.sh` (minimal chroot + unshare), `time_compare.sh` (boot time across docker/podman/microvm if available).

## 5. Tech Stack and Build Policy

- **Language:** C (C99 or later) for all milestones and most demos. A handful of demos use shell scripting.
- **Build:** GNU Make. Each subproject has its own Makefile producing a single binary or a small set. No autotools, no CMake.
- **Compiler:** `gcc` (or `clang` on macOS — both work). All Makefiles set `CC ?= gcc`, `CFLAGS = -Wall -Wextra -std=c99 -pedantic -g`.
- **Linking:** `-pthread` where needed. Demos that use Linux-only headers (`<sys/syscall.h>`, `<linux/seccomp.h>`) are guarded by `#ifdef __linux__` and print a friendly skip message on macOS.
- **No external dependencies** beyond libc and pthreads. Students should not need to install anything.

## 6. README and File Template

Every subproject has a `README.md` with this structure:

```markdown
# <Topic N · Mini-project name>

## Goal
One sentence on what this code does and why it exists.

## Build
make           # builds everything in this directory
make clean     # removes binaries

## Run
./<binary> [args]
./test.sh      # if present

## Files
- src/main.c — entrypoint
- src/<other>.c — <one-line each>

## Bridges to
- Lesson: lessons/Topic_N_<name>.html
- Activity: activities/Topic_N_Activity_2.md
- Assignment: Project N (or Assignment X)

## Notes
Anything platform-specific or surprising.
```

Every C file opens with a uniform comment block:

```c
/*
 * <filename> — CST-315 · Topic N · <milestone|starter|demos>
 * Purpose: <one sentence>
 * Build:   make
 * Run:     ./<binary>
 * Notes:   <platform notes, dependencies>
 */
```

## 7. Test Approach

Each milestone has a `test.sh` that verifies the headline behavior. Tests are smoke-test level — they confirm the code runs and produces an expected output for a single canonical case, not exhaustive correctness. Students can use them as a self-check before submission; the instructor can use them as a sanity gate.

Tests print `PASS` / `FAIL` on each check and exit non-zero on any failure.

## 8. Sharing Boundary (Students vs Instructor)

The `demos/` directory in every topic includes a prominent banner in its README:

> **Instructor materials. Do not distribute to students.** These programs are for in-class demonstration only.

Two practical options to enforce this:

- **Option A (recommended):** One repo, students get instructions to clone only specific paths via sparse-checkout, excluding all `demos/` directories. Cleanest for the instructor.
- **Option B:** Split into two repos — a public student-facing repo and a private instructor repo. More overhead per change.

I recommend Option A. The `.gitignore` and a top-level README clearly tell students what they should and shouldn't pull.

## 9. Production Order

1. **Top-level scaffolding** — repo root, `.gitignore`, `README.md`, `common/`. Done first.
2. **Topic 1 vertical slice** — milestone + starter + demos. Review pause.
3. **Topics 2–8** — same pattern, in syllabus order.
4. **Final pass** — verify cumulative continuity (Topic 5 starter actually builds on Topic 4 milestone), run every `test.sh`, sanity-check every README.
5. **`git init` and first commit** — local only; you push when ready.

## 10. Open Questions for Instructor Approval

1. **Hybrid skeleton confirmed?** Or do you prefer pure isolated or pure cumulative? My recommendation is hybrid; locking this in shapes every starter.
2. **Topic 7 milestone scope.** The injection virus/detector is Assignment 4, not a shell milestone. I propose adding a small hardening commit to the shell (filename sanitization) so Topic 7 still has a shell milestone. Acceptable, or skip the shell side for Topic 7?
3. **Test depth.** Smoke-test level (one canonical case per binary), or more exhaustive (multiple inputs, edge cases)? Smoke-test recommended for time.
4. **Demo language scope.** Most demos in C; a few are shell scripts (Topic 8 container demos). Acceptable, or all-C?
5. **Linux-only demos.** Some demos (seccomp, cgroups, namespaces) only run on Linux. Students will be on Linux per syllabus, but instructor demo machine may be macOS. Default plan: `#ifdef __linux__` guards and a friendly message on macOS. Acceptable?
6. **Sharing mechanism.** Option A (sparse-checkout exclusion) vs Option B (split repos). My recommendation is A.

## 11. Acceptance Criteria for Done

- [ ] `git init` at repo root with initial commit.
- [ ] Every subproject builds cleanly with `make` from its directory.
- [ ] Every subproject has a README following the template.
- [ ] Every milestone's `test.sh` passes.
- [ ] Topic N's starter (when its TODOs are filled with the milestone's code) produces the milestone binary.
- [ ] Topic N's milestone is a strict superset of Topic N-1's milestone, in functionality.
- [ ] Every demo runs (or prints a clean skip message on the wrong platform).
- [ ] Top-level README explains the structure and the sharing boundary.
