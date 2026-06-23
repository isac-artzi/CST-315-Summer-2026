# Topic 2 — In-Class Activities

**Course:** CST-315 Operating Systems Lecture & Lab
**Topic:** Processes and Threads
**Reading:** Stallings, Chapters 3–4
**Companion lesson:** [Topic_2_Processes_Threads.html](../lessons/Topic_2_Processes_Threads.html)
**Master visualization:** [Factory_OS_Master.html](../Factory_OS_Master.html)

These two activities are designed to run in a single class block. Activity 1 (~20 min) cements the fork/exec/wait mental model on paper. Activity 2 (~40 min) produces a minimal working shell loop in C — the kernel of Project 2.

---

## Activity 1 — Trace a fork/exec/wait on the factory floor

**Duration:** ~20 minutes (5 min framing · 10 min trace · 5 min debrief)
**Format:** Pairs, on paper or whiteboard
**Aligned objectives:** 2, 3, 5

### Bridge to assignment
This is the design exercise for Project 2 done with paper and ink. Students who can trace fork/exec/wait by hand here will write the C code in Activity 2 much more quickly.

### Setup
- Print or sketch a simplified factory floor with three zones: Front Desk, Floor (two stations), Wait Queue.
- Each pair gets a stack of small sticky notes to act as "clipboards" (PCBs).
- The command to trace: `ls -l ; touch newfile ; grep TODO main.c`

### Steps
1. **Frame (5 min).** Walk the class through one fork-exec-wait cycle on the projected factory: shell forks a child, child execs, parent waits, child exits, parent resumes. Use real sticky notes on a real whiteboard.
2. **Pairs work (10 min).** Each pair traces the three-command batch line above through the same factory. They must show:
   - The starting state (one clipboard at the front desk: PID 1042 "shell").
   - The state after each fork — new clipboards, new PIDs, parent/child relationships labeled.
   - Which clipboards are on stations vs in the wait queue at each step.
   - The final state when all three children have been reaped.
3. **Debrief (5 min).** Two pairs present their final whiteboards. Compare: how did they handle the requirement that all three commands run concurrently? Did they fork three times before waiting once, or interleave?

### Discussion prompts
- The syllabus requires the batch line's commands to run concurrently. What changes about your trace if you fork-then-wait three times in sequence instead?
- Where is the parent's clipboard during the time it is waiting? What state is it in?
- One of your three children finishes much faster than the others. What does the parent do with that information?

### Instructor notes
- Common pitfall: students fork once, wait, fork again — sequential execution. Push back: "Read the syllabus again. Concurrent means all three are running at once."
- Expected outcomes: by debrief, every pair should articulate that the parent must fork ALL N commands before calling wait, and must call wait in a loop N times to reap all of them.
- Time checkpoint: by minute 12 every pair should be showing three running children simultaneously on their whiteboard.

### Stretch goal
Add error handling: what if the second `exec` fails (e.g., `touch` not found)? Trace what the parent should see and how it should report it. (Hint: the child's exit status.)

---

## Activity 2 — A minimal forking shell in C

**Duration:** ~40 minutes (5 min frame · 25 min code · 10 min test and discuss)
**Format:** Individual at laptop; pairs allowed for debugging
**Aligned objectives:** 1, 2, 5

### Bridge to assignment
This activity produces a working single-command interactive shell that reads a line, forks, execs, waits, and loops. It is the seed of Project 2. Students leave class with a green build and a known-good baseline to extend with batch mode, multi-command concurrency, and CTRL-X exit.

### Setup
- Working Linux/UNIX environment (Topic 1, Activity 2).
- Editor: `vi`, `nano`, or VS Code.
- A `~/CST-315/Topic2` directory ready.

### Steps

#### Part A — Frame the loop (5 min)
On the board, write the loop in pseudocode together. Confirm everyone agrees:

```
loop:
  print prompt
  read line from stdin
  if line is "exit" -> break
  tokenize line into argv[]
  pid = fork()
  if pid == 0:
      execvp(argv[0], argv)
      perror("exec failed"); exit(1)
  else:
      waitpid(pid, &status, 0)
end loop
```

#### Part B — Implement (25 min)

1. Create `~/CST-315/Topic2/lopeShell.c`:

   ```c
   /* lopeShell.c — Topic 2 minimal forking shell */
   #include <stdio.h>
   #include <stdlib.h>
   #include <string.h>
   #include <unistd.h>
   #include <sys/wait.h>

   #define MAX_LINE 1024
   #define MAX_ARGS 64

   int main(void) {
       char line[MAX_LINE];
       char *argv[MAX_ARGS];

       while (1) {
           printf("lopeShell> ");
           fflush(stdout);
           if (!fgets(line, sizeof(line), stdin)) break;
           line[strcspn(line, "\n")] = 0;
           if (strcmp(line, "exit") == 0) break;
           if (line[0] == 0) continue;

           int argc = 0;
           char *tok = strtok(line, " ");
           while (tok && argc < MAX_ARGS - 1) {
               argv[argc++] = tok;
               tok = strtok(NULL, " ");
           }
           argv[argc] = NULL;

           pid_t pid = fork();
           if (pid < 0) { perror("fork"); continue; }
           if (pid == 0) {
               execvp(argv[0], argv);
               perror("execvp");
               exit(1);
           }
           int status;
           waitpid(pid, &status, 0);
       }
       return 0;
   }
   ```

2. Compile and run:

   ```bash
   gcc lopeShell.c -o lopeShell
   ./lopeShell
   ```

3. From the prompt, run `ls`, `pwd`, `whoami`, `ls -l`, `echo hi`, and `exit`. Capture a screenshot.

#### Part C — Instrument and observe (10 min)
4. Add three `printf` statements to log the PIDs:
   - Before `fork()`: `printf("[parent %d] forking...\n", getpid());`
   - In the child branch before `execvp`: `printf("[child %d, parent %d] about to exec %s\n", getpid(), getppid(), argv[0]);`
   - In the parent branch after `waitpid`: `printf("[parent %d] child exited with status %d\n", getpid(), WEXITSTATUS(status));`

5. Recompile, rerun, observe how the same shell PID persists across commands while every child gets a fresh PID. Capture a screenshot.

6. Commit and push to your `CST-315` repo under `Topic2/`.

### Discussion prompts
- Why does the same `lopeShell` PID print every loop, but the child PID is different each time?
- What happens if you replace `waitpid(pid, &status, 0)` with just `wait(NULL)`? Try it.
- What happens if you omit the `exit(1)` after `perror("execvp")`? Why is that dangerous?

### Instructor notes
- Common pitfalls: forgetting `fflush(stdout)` after the prompt (it won't print on some terminals); writing `if (pid == 0) execvp(...)` without the `exit(1)` after — if exec fails, the child falls through and becomes a second shell process competing for the same stdin.
- Expected outcomes: every student gets a working shell. The instrumentation step matters more pedagogically than the working shell; the PIDs make fork/exec/wait concrete.
- Time checkpoint: by minute 30 every student should have completed Part B at minimum.

### Stretch goal
Modify the shell to also recognize the keyword `pid` as a built-in (not a forked command) that prints the shell's own PID and exits without forking. This previews the "shell built-ins vs external commands" distinction needed in Project 2.

---

## Closeout (last 2 minutes of class)

Direct students to:
- Read Stallings Ch. 4 on threads before next class — Topic 3 will assume it.
- Begin sketching the data structure they will use in Project 2 to track multiple concurrent children. (Hint: an array of PIDs plus a counter is sufficient.)
- Form CLC teams for the Producer/Consumer assignment if interested. Teams are limited to two students.
