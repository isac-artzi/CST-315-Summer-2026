# Topic 1 · Activity 2 — Linux Setup Smoke Test and First Commit

**Course:** CST-315 Operating Systems Lecture & Lab
**Duration:** ~40 minutes
**Format:** Individual at your laptop; pairs allowed for debugging

## Aligned objectives
1. Demonstrate familiarity with the features of a UNIX/Linux operating system.
2. Review UNIX/Linux shell scripting.
3. Compile and execute a simple program.
6. Provide technical documentation of design processes, such as screenshots, code snippets, testing and/or other pertinent factors.

## What you will produce
By the end of class, you will have:
1. A verified Linux/UNIX environment with `gcc` and `git` working.
2. A compiled and running `HelloWorld.c`.
3. A private GitHub repository named `CST-315` with `README.md` and `HelloWorld.c` pushed, and the instructor added as a collaborator.

These are three of Project 1's required deliverables.

## Before class
- **Mac users:** Open Terminal.
- **Windows users:** Boot into Debian (or another Linux distro) — separate partition or USB.
- **Everyone:** Have a personal GitHub account ready. Set up SSH keys or install GitHub CLI (`gh auth login`).

## Steps

### Part A — Environment smoke test (10 min)

At your terminal, run each of these and capture a screenshot of the output:

```bash
uname -a
gcc --version
git --version
whoami
pwd
```

If any command is missing, install it now. On Debian: `sudo apt update && sudo apt install build-essential git`. **Do not move on until all five run cleanly.**

### Part B — HelloWorld in C (15 min)

1. Create the project structure:

   ```bash
   mkdir -p ~/CST-315/Assignment1
   cd ~/CST-315/Assignment1
   ```

2. Create `HelloWorld.c` using `vi`, `nano`, or your preferred editor:

   ```c
   /* HelloWorld.c — CST-315 Topic 1 smoke test */
   #include <stdio.h>
   int main(void) {
       printf("Hello, CST-315!\n");
       return 0;
   }
   ```

3. Compile and run:

   ```bash
   gcc HelloWorld.c -o HelloWorld
   ./HelloWorld
   ```

4. Capture a screenshot showing both the compile command and the program's output.

### Part C — Git and GitHub (15 min)

1. Initialize a local repo at `~/CST-315`:

   ```bash
   cd ~/CST-315
   git init
   ```

2. Create `README.md` with a brief self-introduction: your name, major, why you are taking this course, and any prior systems-programming experience (or note that you have none).

3. Stage and commit:

   ```bash
   git add README.md Assignment1/HelloWorld.c
   git commit -m "Topic 1: initial repo, HelloWorld in C"
   ```

4. On GitHub, create a **private** repository named `CST-315`. Add the remote and push:

   ```bash
   git remote add origin git@github.com:<your-username>/CST-315.git
   git branch -M main
   git push -u origin main
   ```

5. Add the instructor as a collaborator on the repo.

6. Share the repository URL in the class channel.

## Discussion prompts
- What did `uname -a` tell you about your machine? Pick two pieces of that output line and explain them.
- When `gcc HelloWorld.c -o HelloWorld` ran, which zones of the factory were active? (Hint: front desk, floor, racks, records room.)
- Why a private repo rather than a public one for this course?

## Stretch goal
Write a `test.sh` shell script that runs five UNIX commands you find useful (try `whoami`, `date`, `uptime`, `df -h`, `ls -lh ~`). Commit it to `~/CST-315/Assignment1/` and push. This is one of Project 1, Part 2's required deliverables — completing it here is a head start.

---
**Companion lesson:** [Topic_1_Overview.html](../lessons/Topic_1_Overview.html)
**Master visualization:** [Factory_OS_Master.html](../Factory_OS_Master.html)
