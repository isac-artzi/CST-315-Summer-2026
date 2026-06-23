# Topic 2 · Activity 2 — A Minimal Forking Shell in C

**Course:** CST-315 Operating Systems Lecture & Lab
**Duration:** ~40 minutes
**Format:** Individual at your laptop; pairs allowed for debugging

## Aligned objectives
1. Develop a shell by implementing the command line interpreter.
2. Implement basic process management.
5. Select and utilize data structures appropriate for a variety of design concepts, principles, tools, and problem-solving strategies.

## What you will produce
A working single-command interactive shell named `lopeShell` that reads a line, forks a child, execs the requested command, waits, and loops. Plus instrumented PID logging that makes fork/exec/wait visible. This is the seed of Project 2.

## Setup
- A working Linux/UNIX environment (from Topic 1, Activity 2).
- An editor: `vi`, `nano`, or VS Code.
- A new directory: `~/CST-315/Topic2`.

## Steps

### Part A — Frame the loop (5 min)
Confirm with the class:

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

### Part B — Implement (25 min)

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

3. From the prompt, run `ls`, `pwd`, `whoami`, `ls -l`, `echo hi`, and `exit`. Screenshot the output.

### Part C — Instrument and observe (10 min)

4. Add three `printf` statements to log PIDs:
   - Before `fork()`: `printf("[parent %d] forking...\n", getpid());`
   - In the child, before `execvp`: `printf("[child %d, parent %d] about to exec %s\n", getpid(), getppid(), argv[0]);`
   - In the parent, after `waitpid`: `printf("[parent %d] child exited with status %d\n", getpid(), WEXITSTATUS(status));`

5. Recompile, rerun, and observe how the shell PID stays the same while every child gets a new PID. Screenshot.

6. Commit and push to your `CST-315` repo under `Topic2/`.

## Discussion prompts
- Why does the same `lopeShell` PID print every loop, but the child PID is different each time?
- What happens if you replace `waitpid(pid, &status, 0)` with just `wait(NULL)`? Try it.
- What happens if you omit the `exit(1)` after `perror("execvp")`? Why is that dangerous?

## Stretch goal
Modify the shell so that the keyword `pid` is a built-in: instead of forking, it directly prints the shell's PID and returns to the prompt. This previews the "built-ins vs external commands" distinction you will need in Project 2.

---
**Companion lesson:** [Topic_2_Processes_Threads.html](../lessons/Topic_2_Processes_Threads.html)
**Master visualization:** [Factory_OS_Master.html](../Factory_OS_Master.html)
