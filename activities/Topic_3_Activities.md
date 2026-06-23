# Topic 3 — In-Class Activities

**Course:** CST-315 Operating Systems Lecture & Lab
**Topic:** Mutual Exclusion and Concurrency
**Reading:** Stallings, Chapters 5–6
**Companion lesson:** [Topic_3_Mutual_Exclusion_Concurrency.html](../lessons/Topic_3_Mutual_Exclusion_Concurrency.html)
**Master visualization:** [Factory_OS_Master.html](../Factory_OS_Master.html)

These two activities cover the conceptual half (diagnose a deadlock by reading a graph) and the practical half (convert an unsafe producer/consumer into a safe one). Together they prepare students for Assignments 2 and 3 and for Project 3's concurrency requirements.

---

## Activity 1 — Diagnose a deadlock from a resource allocation graph

**Duration:** ~20 minutes (5 min framing · 10 min work · 5 min debrief)
**Format:** Pairs, on paper or whiteboard
**Aligned objectives:** 3

### Bridge to assignment
This is the analytical skill Assignment 3 (Deadlock Avoidance) demands: read the activity log, see the cycle, propose a fix. Doing it on a graph first makes it obvious; then the log becomes a tabular version of the same picture.

### Setup
- Print or sketch a small grid with 4 processes (P1–P4) and 4 resources (R1–R4).
- Each pair gets a copy of the following "before" snapshot of the system:

  - P1 holds R1, requests R3
  - P2 holds R2, requests R1
  - P3 holds R3, requests R4
  - P4 holds R4, requests R2

### Steps
1. **Frame (5 min).** Define the two edge types of a resource allocation graph: hold (R → P) and request (P → R). A cycle through both types means deadlock; a cycle entirely within request edges does not.
2. **Pairs draw and diagnose (10 min).** Each pair draws the RAG from the snapshot above, traces edges, identifies whether a cycle exists, and circles the deadlocked processes. Then they propose **two different** ways to resolve the deadlock — each must reference one of Coffman's four conditions.
3. **Debrief (5 min).** One or two pairs present. Cross-check answers. Discuss which strategies are realistic in a production system vs which are academic.

### Discussion prompts
- The syllabus's Assignment 3 uses a timer. Which of Coffman's four conditions does the timer attack? Why is the timer scalable up to a point but not indefinitely?
- If you kill P2 to break the cycle, do you have to also undo any partial work it did? What does that imply about transactions and rollback?
- Could you prevent this deadlock at design time by ordering resource acquisition? How?

### Instructor notes
- Common pitfall: students draw both hold and request edges in the same direction. Be strict: hold goes R→P (resource owned BY process), request goes P→R (process WANTS resource).
- Expected outcomes: every pair should identify the cycle P1→R3→P3→R4→P4→R2→P2→R1→P1.
- Time checkpoint: by minute 12 every pair should be discussing fix strategies, not still drawing.

### Stretch goal
Re-draw the graph assuming R1 has 2 units instead of 1. Does the cycle still mean deadlock? (No — multi-instance resources require a different test, the cycle is necessary but not sufficient.)

---

## Activity 2 — Make the producer/consumer safe with semaphores

**Duration:** ~40 minutes (5 min frame · 25 min code · 10 min measure and discuss)
**Format:** Individual at laptop; pairs allowed for debugging
**Aligned objectives:** 1, 2

### Bridge to assignment
Assignment 2 (Monitors & Semaphores) asks students to implement the same scenario two ways and compare. This activity produces the semaphore version. The monitor version is left for the assignment, so students arrive with one half done and a clear template for the other.

### Setup
- Working Linux/UNIX environment.
- POSIX threads and semaphores: `pthread.h`, `semaphore.h`. Link with `-pthread`.
- A new directory: `~/CST-315/Topic3`.

### Steps

#### Part A — Start broken (5 min)
Open the producer/consumer skeleton from CLC Assignment 1 (or use the version below). Compile and run. Observe that without coordination, the consumer reads inconsistent values or misses items.

```c
/* unsafe.c — racy producer/consumer */
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int buffer = 0;
int produced = 0;

void *producer(void *arg) {
    for (int i = 0; i < 100; i++) {
        buffer = ++produced;
        usleep(1000);
    }
    return NULL;
}
void *consumer(void *arg) {
    int last = 0;
    for (int i = 0; i < 100; i++) {
        if (buffer != last) {
            printf("got %d (gap %d)\n", buffer, buffer - last - 1);
            last = buffer;
        }
        usleep(1000);
    }
    return NULL;
}
int main(void) {
    pthread_t p, c;
    pthread_create(&p, NULL, producer, NULL);
    pthread_create(&c, NULL, consumer, NULL);
    pthread_join(p, NULL); pthread_join(c, NULL);
    return 0;
}
```

Compile: `gcc unsafe.c -o unsafe -pthread && ./unsafe`. Note the gaps (consumer missed values) and any duplicates.

#### Part B — Add the signal light (20 min)
Convert to a bounded-buffer producer/consumer using POSIX semaphores. Target spec:

- Buffer is an array of size 10.
- `empty` semaphore tracks empty slots (initial 10).
- `full` semaphore tracks filled slots (initial 0).
- `mutex` semaphore protects the array (initial 1).

```c
/* safe.c — producer/consumer with semaphores */
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 10
int buf[N]; int in=0, out=0;
sem_t empty, full, mutex;

void *producer(void *arg) {
    for (int i = 1; i <= 50; i++) {
        sem_wait(&empty);
        sem_wait(&mutex);
        buf[in] = i; in = (in + 1) % N;
        sem_post(&mutex);
        sem_post(&full);
    }
    return NULL;
}
void *consumer(void *arg) {
    for (int i = 0; i < 50; i++) {
        sem_wait(&full);
        sem_wait(&mutex);
        int v = buf[out]; out = (out + 1) % N;
        sem_post(&mutex);
        sem_post(&empty);
        printf("consumed %d\n", v);
    }
    return NULL;
}
int main(void) {
    sem_init(&empty, 0, N);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);
    pthread_t p, c;
    pthread_create(&p, NULL, producer, NULL);
    pthread_create(&c, NULL, consumer, NULL);
    pthread_join(p, NULL); pthread_join(c, NULL);
    return 0;
}
```

Compile: `gcc safe.c -o safe -pthread && ./safe`. Verify all 50 items are consumed exactly once, in order.

#### Part C — Measure (15 min)
Use the `time` command to measure both versions. Run each three times. Record real, user, and sys time.

```bash
time ./unsafe > /dev/null
time ./safe > /dev/null
```

Discuss: which is faster? Why? What does the difference (or lack of difference) tell you about the cost of synchronization?

Commit both `unsafe.c` and `safe.c` to `~/CST-315/Topic3/` and push.

### Discussion prompts
- The unsafe version sometimes runs faster. What is it doing instead of synchronization?
- Why three semaphores instead of one? What would break with just one mutex?
- Now imagine doing the same thing with a monitor. Sketch the monitor's interface (signature of its public methods). You'll implement it for real in Assignment 2.

### Instructor notes
- Common pitfalls: forgetting `-pthread` at link time; ordering `sem_wait` calls so that `mutex` is acquired before `empty` (this can deadlock when buffer is full); using global `in`/`out` without `mutex` protection.
- Expected outcomes: every student gets `safe.c` to consume all 50 items in order. Some will see that `unsafe` produces garbage every run; others will get unlucky and see it look correct (race conditions are non-deterministic — a teaching moment).
- Time checkpoint: by minute 30 every student should be running both binaries.

### Stretch goal
Add a third thread (a second consumer) and a fourth (a second producer). Does your `safe.c` still work? Why does this require no code change while the unsafe version gets dramatically worse?

---

## Closeout (last 2 minutes of class)

Direct students to:
- Start Assignment 2 by porting `safe.c` to use a monitor (a struct + a pthread mutex + condition variables). The assignment is the comparison; you already have one side.
- Read Stallings Ch. 6 on deadlock detection before next class. Assignment 3 will lean on it.
- Pull `unsafe.c` from their repo when they need a reminder of what bad concurrency looks like.
