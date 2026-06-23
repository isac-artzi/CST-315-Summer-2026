# Topic 8 · Activity 1 — Architect a Virtualization Solution

**Course:** CST-315 Operating Systems Lecture & Lab
**Duration:** ~20 minutes
**Format:** Pairs, on paper or whiteboard

## Aligned objectives
1. Examine current trends in software virtualization.
2. Analyze virtualization tools.
3. Draw the architecture of the virtualization environment.
4. Convey complex technical information to a wide range of individuals using various presentation strategies.

## What you will produce
For one assigned workload: a one-paragraph description of the dominant constraint, a recommended virtualization technology, and a stack diagram from hardware up.

## Setup
- Whiteboard or paper.
- Your pair will receive one of three workload scenarios:

### W1 — Multi-tenant SaaS hosting
200 customer environments, hard isolation required, mix of Linux and Windows guests, RPO/RTO under 1 hour.

### W2 — CI/CD farm
Ephemeral build environments, 1000s of builds/day, very fast start required, single OS family (Linux), low memory budget.

### W3 — Confidential medical record processing
Strong cryptographic isolation, regulatory compliance (HIPAA), small workload (1–2 instances), tolerable to add overhead.

## Steps

1. **Frame (5 min).** Recap the Topic 8 spectrum: bare metal → Type-1 VMs → Type-2 VMs → microVMs → containers. Each is a point on the isolation/density curve.

2. **Architect (12 min).** For your assigned workload, produce:
   - One paragraph naming the dominant constraint.
   - One recommended virtualization technology (specific product name allowed).
   - A stack diagram from hardware up.

3. **Present (3 min).** Each workload's pair presents.

## Discussion prompts
- For W2, why might containers win even though microVMs offer stronger isolation?
- For W3, what additional question would you ask the customer that the syllabus doesn't list?
- Assignment 5 requires a discussion of stewardship and ethical considerations. For W3, sketch one ethical question the architecture forces you to confront.

## Stretch goal
For your assigned workload, sketch how disaster recovery works. What gets backed up? What's the RTO budget? Where does the backup live? Assignment 5 scores you on these criteria.

---
**Companion lesson:** [Topic_8_Virtual_Environments.html](../lessons/Topic_8_Virtual_Environments.html)
**Master visualization:** [Factory_OS_Master.html](../Factory_OS_Master.html)
