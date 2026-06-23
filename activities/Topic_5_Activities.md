# Topic 5 — In-Class Activities

**Course:** CST-315 Operating Systems Lecture & Lab
**Topic:** Processor Scheduling
**Reading:** Stallings, Chapters 9–10
**Companion lesson:** [Topic_5_Processor_Scheduling.html](../lessons/Topic_5_Processor_Scheduling.html)
**Master visualization:** [Factory_OS_Master.html](../Factory_OS_Master.html)

Activity 1 has students hand-draw Gantt charts for three algorithms on the same workload and compute the metrics. Activity 2 implements Round Robin scheduling decision logic in C — drop-in code for Project 5.

---

## Activity 1 — Hand-draw Gantt charts and compute metrics

**Duration:** ~20 minutes (5 min framing · 12 min work · 3 min debrief)
**Format:** Individual on paper or whiteboard
**Aligned objectives:** 1, 2

### Bridge to assignment
Project 5 asks students to defend their choice of scheduling algorithm. They cannot defend a choice without first feeling the metrics in their hands. Drawing Gantt charts and computing waiting/turnaround makes the trade-offs concrete.

### Setup
- Grid paper or a wide whiteboard.
- The shared workload:

  | PID | Arrival | Burst | Priority |
  |---:|---:|---:|---:|
  | P1 | 0 | 7 | 3 |
  | P2 | 2 | 4 | 1 |
  | P3 | 4 | 1 | 4 |
  | P4 | 5 | 4 | 2 |

  (Lower priority number = higher priority.)

### Steps
1. **Frame (5 min).** Recap waiting time = turnaround - burst, turnaround = completion - arrival. Lower is better. Confirm everyone agrees on the workload.
2. **Draw three Gantts (12 min).** Each student draws Gantt charts for:
   - **FCFS** (first-come, first-served, non-preemptive).
   - **SJF** non-preemptive (shortest known burst).
   - **Round Robin** with quantum = 2.

   Below each Gantt, compute waiting time per process and the average.

3. **Debrief (3 min).** Project the interactive SchedulerSim. Have it produce all three. Match against student computations. Discuss discrepancies.

### Discussion prompts
- Which algorithm gives the lowest average waiting time on this workload? Is that always the case for SJF?
- The Round Robin chart shows context switches. Count them. What would happen to the count if the quantum were 1? If it were 8?
- For Project 5, which of these would you choose to implement and why?

### Instructor notes
- Common pitfall: students forget that SJF non-preemptive only picks among processes that have already arrived. At t=0 only P1 has arrived, so it runs first regardless of burst length.
- Expected outcomes: FCFS avg wait ≈ 6.5; SJF avg wait ≈ 4.0; RR(q=2) avg wait ≈ 6.0. The point: SJF wins on this workload, RR's responsiveness comes at a turnaround cost.
- Time checkpoint: by minute 12 every student should have at least two Gantt charts drawn.

### Stretch goal
Compute the same metrics for **SRT** (preemptive SJF) and for **Priority** (non-preemptive). The SRT chart is the most labor-intensive but illustrates why hardware-supported preemption matters.

---

## Activity 2 — Round Robin decision logic in C

**Duration:** ~40 minutes (5 min frame · 30 min code · 5 min discuss)
**Format:** Individual at laptop; pairs allowed for debugging
**Aligned objectives:** 1, 3, 4

### Bridge to assignment
This activity produces a working Round Robin scheduling kernel in C: a process struct, a circular ready queue, a tick-driven decision loop, and a state-transition helper. Drop the data structures into Project 5; expose `procs`, priority modification, and a parameter system call on top.

### Setup
- Working Linux/UNIX environment.
- A new directory: `~/CST-315/Topic5`.

### Steps

#### Part A — The data structures (5 min)
Write on the board together:

```c
typedef enum { READY, RUNNING, WAITING, TERMINATED } State;

typedef struct Process {
    int pid;
    int arrival;
    int burst;
    int remaining;
    int priority;
    State state;
    struct Process *next;  /* for ready queue linked list */
} Process;
```

#### Part B — Implement the scheduler (30 min)

1. Create `~/CST-315/Topic5/rr.c`:

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
        /* admit any arrived processes */
        for (int i = 0; i < n; i++) {
            if (!admitted[i] && procs[i].arrival <= t) {
                enqueue(&procs[i]);
                admitted[i] = 1;
                printf("t=%2d  admitted PID %d\n", t, procs[i].pid);
            }
        }
        /* if no current, dequeue next */
        if (!cur && head) {
            cur = dequeue();
            cur->state = RUNNING;
            slice_left = QUANTUM;
            printf("t=%2d  scheduled PID %d (remaining=%d)\n", t, cur->pid, cur->remaining);
        }
        /* run one tick */
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

2. Compile and run:

```bash
gcc rr.c -o rr
./rr
```

3. Verify the trace: P1 runs t=0..2, quantum expires, P2 admitted at t=2 then scheduled, etc. Screenshot.

#### Part C — Parameter exposure (5 min)
Discuss as a class: how would you expose `QUANTUM` so the shell could change it at runtime?
- Option 1: an environment variable read at startup.
- Option 2: a configuration file the scheduler re-reads.
- Option 3: a custom system call (the syllabus's objective 4) — would require kernel module work or a user-space IPC mechanism.

Pick one and sketch the call signature.

Commit `rr.c` to `~/CST-315/Topic5/` and push.

### Discussion prompts
- The current code uses a linked list. For a real system with 10,000 processes, what's wrong with that?
- What happens if a process is preempted while holding a resource (a mutex)? Your scheduler doesn't know about that — should it?
- Project 5 requires you to defend your algorithm choice. Given this workload, would you defend RR? Justify in one paragraph.

### Instructor notes
- Common pitfalls: not marking admitted processes (causes infinite re-enqueueing); not re-checking arrivals on every tick (P4 arriving at t=5 must be admitted, not skipped); incrementing `t` before running the tick (off-by-one in metrics).
- Expected outcomes: the trace should show 7 quantum expiries before all four processes complete. Total time ≈ 16.
- Time checkpoint: by minute 30 every student should have a working trace.

### Stretch goal
Add I/O simulation: assume P2 makes an I/O call after 2 ticks of running. Modify the loop to move it to a wait queue, simulate I/O completing 3 ticks later, and re-admit it. Track the impact on metrics.

---

## Closeout (last 2 minutes of class)

Direct students to:
- Read Stallings Ch. 10 on multiprocessor scheduling tonight. The 7-week pace assumes this between sessions.
- Begin sketching the queue data structure for Project 5. The linked list works for small loads; consider a heap for the priority variant.
- Plan time for the parameter system call — it is non-trivial and easy to leave until last.
