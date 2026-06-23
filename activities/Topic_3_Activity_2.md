# Topic 3 · Activity 2 — Make the Producer/Consumer Safe with Semaphores

**Course:** CST-315 Operating Systems Lecture & Lab
**Duration:** ~40 minutes
**Format:** Individual at your laptop; pairs allowed for debugging

## Aligned objectives
1. Differentiate between monitors and semaphores.
2. Create a C program that implements a specified scenario.

## What you will produce
A racy `unsafe.c` (to see the problem) and a corrected `safe.c` using POSIX semaphores — a bounded buffer of size 10 with an `empty` semaphore, a `full` semaphore, and a `mutex`. Plus timing measurements that compare both.

This produces the semaphore half of Assignment 2 (Monitors & Semaphores).

## Setup
- A working Linux/UNIX environment.
- POSIX headers: `pthread.h`, `semaphore.h`. Link with `-pthread`.
- A new directory: `~/CST-315/Topic3`.

## Steps

### Part A — Start broken (5 min)
Create `unsafe.c`:

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

Compile and run:

```bash
gcc unsafe.c -o unsafe -pthread
./unsafe
```

Observe gaps and duplicates. Capture a screenshot.

### Part B — Add the signal light (20 min)
Create `safe.c`:

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

Compile and run:

```bash
gcc safe.c -o safe -pthread
./safe
```

Verify all 50 items are consumed exactly once, in order. Capture a screenshot.

### Part C — Measure (15 min)
Run `time` on each, three times:

```bash
time ./unsafe > /dev/null
time ./safe > /dev/null
```

Record real, user, and sys time in a table. Discuss the difference.

Commit both `unsafe.c` and `safe.c` to `~/CST-315/Topic3/` and push.

## Discussion prompts
- The unsafe version sometimes runs faster. What is it doing instead of synchronization?
- Why three semaphores instead of one? What would break with just one mutex?
- Sketch the public interface of a `BoundedBuffer` monitor that would replace this. (You will implement it for Assignment 2.)

## Stretch goal
Add a second producer and a second consumer (4 threads total). Does `safe.c` still work without code changes? Why does this require no code change while `unsafe.c` gets dramatically worse?

---
**Companion lesson:** [Topic_3_Mutual_Exclusion_Concurrency.html](../lessons/Topic_3_Mutual_Exclusion_Concurrency.html)
**Master visualization:** [Factory_OS_Master.html](../Factory_OS_Master.html)
