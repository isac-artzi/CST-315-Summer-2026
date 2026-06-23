# Topic 5 · Activity 1 — Hand-Draw Gantt Charts and Compute Metrics

**Course:** CST-315 Operating Systems Lecture & Lab
**Duration:** ~20 minutes
**Format:** Individual on paper or whiteboard

## Aligned objectives
1. Implement process scheduling algorithms.
2. Examine the effect of different parameters.

## What you will produce
Three Gantt charts for the same workload (FCFS, SJF, RR with quantum 2) and computed average waiting time for each.

## Setup
- Grid paper or whiteboard.
- The shared workload:

  | PID | Arrival | Burst | Priority |
  |---:|---:|---:|---:|
  | P1 | 0 | 7 | 3 |
  | P2 | 2 | 4 | 1 |
  | P3 | 4 | 1 | 4 |
  | P4 | 5 | 4 | 2 |

  (Lower priority number = higher priority.)

Formulas: waiting time = turnaround - burst; turnaround = completion - arrival.

## Steps

1. **Frame (5 min).** Confirm the formulas with the instructor.

2. **Draw three Gantts (12 min).** For the same workload, produce Gantt charts and compute average waiting time for:
   - **FCFS** — first-come, first-served, non-preemptive.
   - **SJF** non-preemptive — among processes that have arrived, pick the shortest burst.
   - **Round Robin** with quantum = 2 — rotate through the ready queue.

3. **Debrief (3 min).** Compare your numbers against the projected SchedulerSim output.

## Discussion prompts
- Which algorithm has the lowest average waiting time here? Is SJF always the winner?
- Count the context switches in your RR chart. What would happen if quantum were 1? If it were 8?
- For Project 5, which of these would you choose to implement and why?

## Stretch goal
Compute the same metrics for **SRT** (preemptive SJF) and for **Priority** (non-preemptive). Which produces the lowest average waiting time on this workload?

---
**Companion lesson:** [Topic_5_Processor_Scheduling.html](../lessons/Topic_5_Processor_Scheduling.html)
**Master visualization:** [Factory_OS_Master.html](../Factory_OS_Master.html)
