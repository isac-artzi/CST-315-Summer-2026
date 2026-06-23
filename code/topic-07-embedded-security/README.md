# Topic 7 — Embedded Systems and Security

Three subprojects:

- **`milestone/`** — Three binaries:
  - `lopeShell` v3: shell with an input-sanitization pass (rejects argv containing shell metacharacters).
  - `virus`: command-injection virus that rewrites argv before `execvp`. Required by Assignment 4.
  - `scan`: pattern-based scanner that walks a directory and flags suspicious files. Required by Assignment 4.
- **`starter/`** — cumulative with TODOs on the sanitizer, virus payload, and scanner walker.
- **`demos/`** — INSTRUCTOR ONLY. `aslr_demo`, `stack_canary_demo`, `seccomp_demo` (Linux), `setuid_demo`.

## Aligned course materials

- Lesson: `lessons/Topic_7_Embedded_Security.html`
- Activity 1: `activities/Topic_7_Activity_1.md`
- Activity 2: `activities/Topic_7_Activity_2.md`
- Project: Assignment 4 (Injection Virus); Project 6 hardening
