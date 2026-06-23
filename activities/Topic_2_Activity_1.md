# Topic 2 · Activity 1 — Trace a fork/exec/wait on the Factory Floor

**Course:** CST-315 Operating Systems Lecture & Lab
**Duration:** ~20 minutes
**Format:** Pairs, on paper or whiteboard

## Aligned objectives
2. Implement basic process management.
3. Differentiate between processes and threads.
5. Select and utilize data structures appropriate for a variety of design concepts, principles, tools, and problem-solving strategies.

## What you will produce
A whiteboard or paper trace showing every process, its state, and its location on the factory floor for the duration of one multi-command batch line.

## Setup
- A simplified floor plan with three zones: Front Desk, Floor (two stations), Wait Queue.
- A stack of sticky notes (your "clipboards"/PCBs).
- The command to trace: `ls -l ; touch newfile ; grep TODO main.c`

## Steps

1. **Watch the demo (5 min).** The instructor traces one fork-exec-wait cycle on the projected factory.

2. **Trace the batch line (10 min).** With your partner, trace `ls -l ; touch newfile ; grep TODO main.c` through the factory. Show:
   - The starting state: one sticky note at the front desk labeled `PID 1042 — shell`.
   - The state after each `fork()` — new sticky notes with new PIDs, parent/child relationships labeled.
   - Which sticky notes (processes) are on stations and which are in the wait queue at each step.
   - The final state when all three children have been reaped and the prompt returns.

3. **Present (5 min).** Two pairs share their final whiteboards. Discuss differences.

## Discussion prompts
- The syllabus requires the batch line's commands to run concurrently. What changes about your trace if you fork-then-wait three times in sequence instead?
- Where is the parent's sticky note during the time it is waiting? What state is it in?
- One of your three children finishes much faster than the others. What does the parent do with that information?

## Stretch goal
Add error handling: what if `touch` is not found and its `exec` call fails? Trace what the parent should observe and how it should report the failure. (Hint: the child's exit status.)

---
**Companion lesson:** [Topic_2_Processes_Threads.html](../lessons/Topic_2_Processes_Threads.html)
**Master visualization:** [Factory_OS_Master.html](../Factory_OS_Master.html)
