# Welcome to CST-315: Operating Systems Lecture & Lab

Welcome to the Summer session of CST-315. This course covers the concepts, structure, and mechanisms of modern operating systems, including how memory, processors, networks, and security interact with programming languages and architectures. The lab component reinforces lecture material through hands-on development of a UNIX/Linux shell that implements process management, file management, and I/O management.

## Format Note

This is the same course as the standard 15-week offering, condensed into a 7-week Summer session. To cover the full curriculum, we will meet for twice the number of contact hours per week. Pace yourself accordingly: each week of this session corresponds to roughly two weeks of the standard schedule. You should expect a higher weekly workload and a shorter turnaround between topics. Plan your study and project time on a weekly basis from day one.

## What You Will Learn

By the end of the course, you will be able to:

- Work proficiently in a UNIX/Linux environment and write shell scripts.
- Build a functional command-line interpreter in C/C++, following the SDLC.
- Implement and reason about processes, threads, and concurrency primitives (semaphores, monitors).
- Apply scheduling algorithms and design a custom short-term scheduler.
- Manage memory through paging, page tables, and virtual memory techniques.
- Design and implement a hierarchical file system with full create/modify/delete lifecycle support.
- Analyze embedded systems, common security threats, and basic malware behavior.
- Evaluate virtualization technologies and articulate their architectural trade-offs.

## Relevance to Industry

Operating systems are the foundation on which every production system runs. Engineers working in cloud infrastructure, embedded devices, mobile platforms, security, distributed systems, and high-performance computing all rely on the concepts in this course daily. Concurrency, scheduling, memory management, file systems, and virtualization are not academic curiosities; they show up in container runtimes, database engines, real-time control software, and the kernels behind every cloud provider. The shell project, in particular, exercises the same systems-programming skills used to build production tooling, observability agents, and platform services.

## Tooling and Environment

You will develop in C or C++ (Java may be permitted with instructor approval, with the caveat that it lacks the low-level memory features needed for some projects). The syllabus lists CLion as the required IDE; for this session, **VSCode or any other IDE capable of building and running C code is acceptable**. Use whatever you are most productive in. Mac users already have BSD UNIX available natively. Windows users are required to run Linux (Debian preferred) either on a separate partition or from a bootable USB; testing inside a Windows-only environment is not sufficient.

All code is managed in **GitHub**. Each project includes a private GitHub repository and a `Readme.md` written in Markdown. Get your repository set up in the first week so it does not become a blocker later.

> **Note:** The syllabus and resource guides reference BitBucket throughout. For this session, **GitHub replaces BitBucket everywhere it appears** — for repository hosting, code submission, and instructor links. Read every "BitBucket" mention as "GitHub."

## Assignments and Their Broader Context

The work in this course is structured around two parallel tracks that reinforce each other.

**The Shell Project** is a seven-part incremental build. Each part is a standalone deliverable, but the parts integrate into a single working UNIX/Linux shell by the end of the term:

1. **Familiarity with UNIX/Linux** — environment setup, basic commands, and a first C program compiled and committed to GitHub.
2. **Command Line Interpreter** — implement the core interpreter using `fork()`, `execv()`, `wait()`, and `waitpid()`, supporting both interactive and batch modes.
3. **Improved Command Line Interpreter** — extend the interpreter with at least two new features and address feedback on design and style.
4. **Pager — Virtual Memory Manager** — define page tables, frame tables, allocation policy, demand paging, page replacement, and fault handling.
5. **Short-Term Process Scheduler** — implement a scheduling algorithm of your choice, manage process states, and expose process inspection commands (`procs`, `procs -a`, etc.).
6. **File System Manager** — implement a hierarchical file system with create, rename, edit, delete, move, duplicate, search, and tree-display operations on both files and directories.
7. **Shell Presentation** — 5–7 minute screencast plus slide deck demonstrating the completed shell and explaining the algorithms behind it.

This structure mirrors how real systems software is built: incrementally, with each layer built on the previous one, and with each iteration verified before the next is added.

**Topic Assignments** are focused exercises that isolate a specific OS concept:

- *Producer and Consumer* (CLC) — concurrency using POSIX threads without semaphores.
- *Monitors and Semaphores* — implement the same scenario two ways and justify a recommendation.
- *Deadlock Avoidance* — timer-based deadlock breaking, with a written assessment of scalability and limitations.
- *Injection Virus* — design a command-injection virus and a corresponding detector.
- *Assessing Virtualization Software* — mini research task comparing five virtualization products against defined criteria.

These map directly to problems engineers solve in industry: avoiding race conditions, preventing deadlocks, recognizing attack patterns, and selecting infrastructure technologies.

**Weekly Lab Questions and Quizzes** ensure you stay current with the reading from Stallings' *Operating Systems: Internals and Design Principles*. Treat these as low-stakes opportunities to confirm your understanding before it is tested in the projects.

**Collaboration policy:** Every project and the Producer/Consumer assignment may be completed as a Collaborative Learning Community (CLC) of up to two students. CLC is optional, and teams do not need to remain the same across the term. The remaining assignments are individual.

## Modern Concepts

Where appropriate, I will supplement the textbook with material on more recent developments in operating systems, so the course reflects current industry practice. Topics may include containerization and container runtimes, modern Linux kernel features (eBPF, io_uring, cgroups v2), microkernel and unikernel approaches, modern schedulers, hardware-assisted virtualization, confidential computing, and current security mechanisms. These additions are intended to connect classical OS theory to the systems you will actually encounter in your careers.

## How to Be Successful

- **Set up your environment before the first lecture.** Install Linux (if needed), install your IDE of choice (CLion, VSCode, or equivalent), confirm your GitHub access, and compile a Hello World in C. Lost setup time in week 1 cascades through the rest of the session.
- **Stay on the weekly rhythm.** Falling one week behind in a 7-week session is equivalent to falling two weeks behind in the regular schedule. Recovery is difficult.
- **Read before lecture.** The textbook is dense. Skim the assigned chapters before class so lecture time reinforces rather than introduces the material.
- **Treat the shell as one project, not seven.** Each part integrates with the others. Design with future parts in mind — for example, your memory manager (Project 4) and file system (Project 6) must not overlap in storage, or your shell will collapse with data loss.
- **Commit early and often.** All code and documentation lives in GitHub. Use version control from day one, not the night before the deadline.
- **Test on Linux/UNIX, not just inside your IDE.** Your shell must run in a real shell environment. Develop and test there from the beginning.
- **Document as you go.** Screenshots, code snippets, flowcharts, and design notes are required deliverables. Capture them while you build, not at the end.
- **Use the lab questions.** They are designed to surface gaps in your understanding before they hurt you on quizzes or projects. Do not skip them.
- **Ask early.** If something is unclear, raise it in class, on the discussion board, or in office hours. Compressed schedules punish silent confusion.
- **Know which work is collaborative.** CLC is optional for the projects and for the Producer/Consumer assignment. The remaining assignments are individual. Confirm before you team up.

I am looking forward to a productive term. Let's get started.
