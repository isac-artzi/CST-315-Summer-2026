# Topic 3 · Activity 1 — Diagnose a Deadlock from a Resource Allocation Graph

**Course:** CST-315 Operating Systems Lecture & Lab
**Duration:** ~20 minutes
**Format:** Pairs, on paper or whiteboard

## Aligned objectives
3. Assess the efficiency of a timer to solve deadlock situations.

## What you will produce
A drawn resource allocation graph, an explicit identification of any cycle, and two proposed fixes — each justified by referencing one of Coffman's four conditions for deadlock.

## Setup
- A grid with 4 processes (P1–P4) and 4 resources (R1–R4).
- The following "before" snapshot:
  - P1 holds R1, requests R3
  - P2 holds R2, requests R1
  - P3 holds R3, requests R4
  - P4 holds R4, requests R2

## Steps

1. **Frame (5 min).** Hold edges go from resource to process (R → P). Request edges go from process to resource (P → R). A cycle that alternates both edge types means deadlock for single-instance resources.

2. **Draw and diagnose (10 min).** With your partner:
   - Draw the four processes as circles and the four resources as squares.
   - Add the hold and request edges from the snapshot above.
   - Identify any cycle and circle the deadlocked processes.
   - Propose **two different** ways to resolve the deadlock. Each fix must attack a different Coffman condition (mutual exclusion, hold-and-wait, no preemption, or circular wait). Write one sentence per fix explaining which condition it negates.

3. **Present (5 min).** Two pairs share their graphs and proposed fixes.

## Discussion prompts
- Assignment 3 uses a timer to break deadlocks. Which Coffman condition does the timer attack? Why is the timer scalable up to a point but not indefinitely?
- If you kill P2 to break the cycle, do you have to undo any partial work it did? What does that imply about transactions and rollback?
- Could you prevent this deadlock at design time by ordering resource acquisition? How?

## Stretch goal
Re-draw the graph assuming R1 has 2 units instead of 1 (multi-instance resource). Does the cycle still mean deadlock? Why or why not?

---
**Companion lesson:** [Topic_3_Mutual_Exclusion_Concurrency.html](../lessons/Topic_3_Mutual_Exclusion_Concurrency.html)
**Master visualization:** [Factory_OS_Master.html](../Factory_OS_Master.html)
