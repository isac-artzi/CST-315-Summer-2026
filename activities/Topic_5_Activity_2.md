# Topic 5 · Activity 2 — Round Robin Decision Logic in C

**Course:** CST-315 Operating Systems Lecture & Lab
**Duration:** ~40 minutes
**Format:** Individual at your laptop; pairs allowed for debugging

## Aligned objectives
1. Implement process scheduling algorithms.
3. Develop a custom Unix/Linux scheduler.
4. Modify a parameter for the scheduler by adding a system call.

## What you will produce
A working Round Robin scheduler kernel in C: a process struct, a circular ready queue, a tick-driven decision loop, and state transitions. Drop these data structures into Project 5.

## Setup
- Working Linux/UNIX environment.
- A new directory: `~/CST-315/Topic5`.

## Steps

### Part A — Data structures (5 min)

Confirm the shape together:

```c
typedef enum { READY, RUNNING, WAITING, TERMINATED } State;

typedef struct Process {
    int pid;
    int arrival;
    int burst;
    int remaining;
    int priority;
    State state;
    struct Process *next;
} Process;
```

### Part B — Implement (30 min)

Create `~/CST-315/Topic5/rr.c`:

```c
/* rr.c — Round Robin scheduler kernel */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { READY, RUNNING, WAITING, TERMINATED } State;

typedef struct Process {
    int pid;
    int arrival;
    int burst;
    int remaining;
    int priority;
    State state;
    struct Process *next;
} Process;

#define QUANTUM 2

Process *head = NULL, *tail = NULL;

void enqueue(Process *p) {
    p->next = NULL;
    if (!head) head = tail = p;
    else { tail->next = p; tail = p; }
    p->state = READY;
}
Process *dequeue(void) {
    if (!head) return NULL;
    Process *p = head;
    head = head->next;
    if (!head) tail = NULL;
    p->next = NULL;
    return p;
}

int main(void) {
    Process procs[] = {
        {1, 0, 7, 7, 3, READY, NULL},
        {2, 2, 4, 4, 1, READY, NULL},
        {3, 4, 1, 1, 4, READY, NULL},
        {4, 5, 4, 4, 2, READY, NULL},
    };
    int n = sizeof(procs)/sizeof(procs[0]);
    int admitted[n]; memset(admitted, 0, sizeof(admitted));

    int t = 0, completed = 0;
    Process *cur = NULL;
    int slice_left = 0;

    while (completed < n) {
        for (int i = 0; i < n; i++) {
            if (!admitted[i] && procs[i].arrival <= t) {
                enqueue(&procs[i]);
                admitted[i] = 1;
                printf("t=%2d  admitted PID %d\n", t, procs[i].pid);
            }
        }
        if (!cur && head) {
            cur = dequeue();
            cur->state = RUNNING;
            slice_left = QUANTUM;
            printf("t=%2d  scheduled PID %d (remaining=%d)\n", t, cur->pid, cur->remaining);
        }
        if (cur) {
            cur->remaining--;
            slice_left--;
            t++;
            if (cur->remaining == 0) {
                printf("t=%2d  PID %d terminated\n", t, cur->pid);
                cur->state = TERMINATED;
                cur = NULL;
                completed++;
            } else if (slice_left == 0) {
                printf("t=%2d  PID %d quantum expired, requeue\n", t, cur->pid);
                enqueue(cur);
                cur = NULL;
            }
        } else {
            t++;
        }
    }
    printf("\nAll processes complete at t=%d\n", t);
    return 0;
}
```

Compile and run:

```bash
gcc rr.c -o rr
./rr
```

Screenshot the trace.

### Part C — Parameter exposure (5 min)

How would you let the shell change `QUANTUM` at runtime? Pick one and write down the call signature:
- Environment variable read at startup.
- Configuration file re-read on signal.
- Custom system call (objective 4).

Commit `rr.c` to `~/CST-315/Topic5/` and push.

## Discussion prompts
- The current code uses a linked list. For 10,000 processes, what's wrong with that?
- What happens if a process is preempted while holding a mutex? Should the scheduler know?
- Given this workload, would you defend RR as the right algorithm for Project 5? Justify in one paragraph.

## Stretch goal
Simulate I/O: when P2 has run for 2 ticks, move it to a wait queue. Re-admit it 3 ticks later. Track how this changes the metrics.

---
**Companion lesson:** [Topic_5_Processor_Scheduling.html](../lessons/Topic_5_Processor_Scheduling.html)
**Master visualization:** [Factory_OS_Master.html](../Factory_OS_Master.html)
