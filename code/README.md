# CST-315 Code Repository

Reference implementations, student starters, and instructor demos for **CST-315 Operating Systems Lecture & Lab** (Summer 7-week session).

## Layout

```
code/
├── README.md                 ← this file
├── .gitignore
├── LICENSE.txt
├── publish.sh                ← helper to copy a topic to a student-facing repo
├── common/                   ← shared headers across topics
├── topic-01-overview/
│   ├── README.md             ← topic orientation
│   ├── milestone/            ← reference shell milestone for this topic
│   ├── starter/              ← student-facing skeleton (TODOs scoped to this topic)
│   └── demos/                ← INSTRUCTOR ONLY — extra concepts from this topic
├── topic-02-processes-threads/
└── ... (topics 3–8)
```

Each topic has the same three subdirectories:

- **`milestone/`** — the working reference implementation of the shell at this milestone. It is *cumulative*: Topic N's milestone contains everything from Topic N−1 plus the new work for Topic N. By Topic 8 it is the complete shell required by Project 7.
- **`starter/`** — what students receive. Same cumulative scope as the milestone, but the *new* work for the current topic is replaced with `// TODO` markers. Prior code is the milestone's reference (students can replace it with their own).
- **`demos/`** — small instructor-only programs that illustrate other concepts covered in the lesson. **Not distributed to students.**

## Build

Every subproject is independent. From any `milestone/`, `starter/`, or `demos/` directory:

```bash
make           # build everything
make clean     # remove binaries
make test      # run smoke test (milestones only)
```

Compiler defaults to `cc` (which is `gcc` on Linux and Apple-clang on macOS). Override with `CC=gcc make`. C99 with `-Wall -Wextra -pedantic -g`.

## Portability

All shell milestones and student starters use POSIX-only APIs and are tested on **macOS** (Apple silicon) and **Linux** (Debian). Anywhere a POSIX construct is silently broken on one OS, the code uses the portable alternative (e.g., named semaphores rather than `sem_init` with `pshared=0`, which is deprecated on Apple platforms).

Instructor demos may use Linux-only features (`/proc`, namespaces, `seccomp`, `epoll`). Those files guard with `#ifdef __linux__` and print a friendly skip message on macOS.

## Publishing to students

```bash
./publish.sh 1 ~/CST-315-student-repo
```

Copies `topic-01-overview/milestone/` and `topic-01-overview/starter/` (plus the topic README) into the destination directory. **`demos/` is never copied.** Run this each week after that week's topic is finalized; commit and push the destination repo to the student-facing GitHub remote.

## Topic index

| Topic | Title | Milestone binary | Aligns with |
|---:|---|---|---|
| 1 | Overview | `hello` | Project 1 |
| 2 | Processes and Threads | `lopeShell` (basic) | Project 2 |
| 3 | Mutual Exclusion and Concurrency | `lopeShell` (concurrent) | Project 3, Assignment 2, 3 |
| 4 | Memory Management and Virtual Memory | `lopeShell` + `pager` | Project 4 |
| 5 | Processor Scheduling | `lopeShell` + `scheduler` | Project 5 |
| 6 | I/O and File Management | `lopeShell` + `fs` | Project 6 |
| 7 | Embedded Systems and Security | hardened `lopeShell` + `virus`/`scan` | Assignment 4 |
| 8 | Virtual Environments | final `lopeShell` + `Dockerfile` | Project 7 |
