# Topic 1 — In-Class Activities

**Course:** CST-315 Operating Systems Lecture & Lab
**Topic:** Overview of Computer Systems and Operating Systems
**Reading:** Stallings, Chapters 1–2
**Companion lesson:** [Topic_1_Overview.html](../lessons/Topic_1_Overview.html)
**Master visualization:** [Factory_OS_Master.html](../Factory_OS_Master.html)

These two activities are designed to be run in a single class block. Activity 1 (~20 min) primes the metaphor and surfaces students' mental models. Activity 2 (~40 min) is hands-on and produces deliverables that feed directly into Project 1. Neither is graded; both are scaffolding.

---

## Activity 1 — Sketch your computer as a factory

**Duration:** ~20 minutes (5 min framing · 10 min work · 5 min debrief)
**Format:** Individual sketch, then pair share
**Aligned objectives:** 1, 5, 7

### Bridge to assignment
Project 1, Part 1 asks students to "choose five features of the operating system and explain what you think happens behind the scenes." This activity warms up that exact muscle — narrating an unseen mechanism — and gives them a shared metaphor to hang their narration on.

### Setup
- Hand each student a blank floor-plan worksheet, or have them sketch on paper. (A printable PDF version of the empty floor plan can be produced from the master visualization on request.)
- Project the master visualization (`Factory_OS_Master.html`) on the screen, set to "Whole Factory."

### Steps
1. **Frame (5 min).** Walk the class through the master visualization at the front of the room. Name each zone aloud and point at the corresponding factory element. Emphasize: this is the same diagram we will reference all term.
2. **Sketch (10 min).** Each student draws a simplified version of the factory on paper, labels the zones, and then writes one to two sentences answering: *"What happens, behind the scenes, when I double-click an icon to open an app?"* They should walk the action through their drawn factory, naming which zones are involved.
3. **Pair share (5 min).** Students pair up, swap drawings, and compare paths. Where do the two narratives diverge? Which zones did each forget to involve?

### Discussion prompts
- Which zone was hardest to draw or explain? Why?
- Where in your factory does the answer to "where did my window go when I closed it" live?
- The master diagram shows nine zones. How many of them did you actually involve in your double-click story? What does that suggest?

### Instructor notes
- Common pitfall: students draw a single zone ("the OS") rather than the decomposition. Push back: "Where in the OS? Which step?"
- Expected outcomes: most students will neglect the kernel/manager's office and the warehouse. That gap previews Topics 2 and 4.
- Time checkpoint: by minute 15, everyone should have at least the front desk, the floor, and the racks labeled.

### Stretch goal
Add a second story: *"What happens when my browser runs out of memory and starts swapping?"* Trace this through the warehouse. This previews Topic 4.

---

## Activity 2 — Linux setup smoke test and first commit

**Duration:** ~40 minutes (10 min setup verification · 20 min hands-on · 10 min commit and verify)
**Format:** Individual at laptop; pairs allowed for debugging
**Aligned objectives:** 1, 2, 3, 6

### Bridge to assignment
This activity produces three of Project 1's required deliverables: a working Linux/UNIX environment, a compiled `HelloWorld.c`, and an initialized GitHub repository with a first commit. Students leave class with a green build and a pushed commit, removing the most common Project 1 blocker.

### Setup
Required before class:
- Mac users: open Terminal.
- Windows users: have Debian (or another Linux distro) booted from a separate partition or USB.
- Everyone: a personal GitHub account, GitHub CLI (`gh`) or SSH keys configured.

Required in class:
- One large screen for the instructor to model each step.
- Slack/Discord/Teams channel open for fast unblock help.

### Steps

#### Part A — Environment smoke test (10 min)
At the terminal, each student runs and captures (screenshot) the output of:
```bash
uname -a
gcc --version
git --version
whoami
pwd
```
If any command is missing, install it now. Do not move on until all five run cleanly.

#### Part B — HelloWorld in C (15 min)
1. Create the project structure:
   ```bash
   mkdir -p ~/CST-315/Assignment1
   cd ~/CST-315/Assignment1
   ```
2. Write `HelloWorld.c` using `vi`, `nano`, or VS Code:
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
4. Capture a screenshot showing the command and its output.

#### Part C — Git + GitHub (15 min)
1. Initialize a local repo at `~/CST-315`:
   ```bash
   cd ~/CST-315
   git init
   ```
2. Create a `README.md` with a brief self-introduction (name, major, why you're taking this course, one prior systems-programming experience or none).
3. Stage and commit:
   ```bash
   git add README.md Assignment1/HelloWorld.c
   git commit -m "Topic 1: initial repo, HelloWorld in C"
   ```
4. Create a **private** repository on GitHub named `CST-315`. Add the remote and push:
   ```bash
   git remote add origin git@github.com:<your-username>/CST-315.git
   git branch -M main
   git push -u origin main
   ```
5. Add the instructor as a collaborator on the GitHub repo.
6. Share the repository URL in the class channel.

### Discussion prompts
- What did `uname -a` tell you about your machine? Read at least two pieces of that output line aloud and explain them.
- When `gcc HelloWorld.c -o HelloWorld` ran, what zones of the factory were active? (Hint: front desk, floor, racks, records room.)
- Why a private repo rather than public for this course?

### Instructor notes
- Common pitfalls: Windows users still trying to compile inside WSL when the syllabus expects a real Linux boot; missing SSH keys for GitHub; `gcc` not installed on a fresh Debian (install with `sudo apt install build-essential`).
- Expected outcomes: by the end of class, every student has pushed a commit and the instructor has been added as collaborator. If a student is blocked on Git, the priority is getting a green build; Git can be debugged in office hours.
- Time checkpoint: by minute 25, every student should have run `./HelloWorld` successfully. Push the Git work after.

### Stretch goal
Write a `test.sh` shell script that runs five UNIX commands you find useful (e.g., `whoami`, `date`, `uptime`, `df -h`, `ls -lh ~`). Commit it to `~/CST-315/Assignment1/` and push. This is one of Project 1, Part 2's deliverables — completing it here is a head start.

---

## Closeout (last 2 minutes of class)

Direct students to:
- Continue reading Stallings Ch. 1–2 if they have not yet.
- Begin drafting Project 1's Part 1 feature-explanation table tonight while the lesson is fresh — they already wrote a draft narration in Activity 1.
- Bring questions from setup to the next class or to office hours; do not let environment issues linger past Topic 2.
