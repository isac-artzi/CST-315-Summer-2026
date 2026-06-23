# Topic 8 — In-Class Activities

**Course:** CST-315 Operating Systems Lecture & Lab
**Topic:** Virtual Environments
**Reading:** Stallings, Chapter 14
**Companion lesson:** [Topic_8_Virtual_Environments.html](../lessons/Topic_8_Virtual_Environments.html)
**Master visualization:** [Factory_OS_Master.html](../Factory_OS_Master.html)

Activity 1 walks the architecture-decision exercise that Assignment 5 demands. Activity 2 wraps the shell project in a container so it can be demoed portably in Project 7.

---

## Activity 1 — Architect a virtualization solution

**Duration:** ~20 minutes (5 min framing · 12 min work · 3 min debrief)
**Format:** Pairs, on paper or whiteboard
**Aligned objectives:** 1, 2, 3, 4

### Bridge to assignment
Assignment 5 asks students to pick a workload, evaluate five virtualization products, score them on a defined criteria set, and recommend one. This activity does that exercise in miniature for a fixed workload so the framework is internalized before they tackle the full assignment.

### Setup
- Whiteboard or large paper.
- Three workload scenarios (assign one to each pair):

  - **W1 — Multi-tenant SaaS hosting.** 200 customer environments, hard isolation required, mix of Linux and Windows guests, RPO/RTO under 1 hour.
  - **W2 — CI/CD farm.** Ephemeral build environments, 1000s of builds/day, very fast start required, single OS family (Linux), low memory budget.
  - **W3 — Confidential medical record processing.** Strong cryptographic isolation, regulatory compliance (HIPAA), small workload (1–2 instances), tolerable to add overhead.

### Steps
1. **Frame (5 min).** Recap the Topic 8 spectrum: bare metal → Type-1 VMs → Type-2 VMs → microVMs → containers. Each is a point on the isolation/density curve.

2. **Architect (12 min).** For your assigned workload, your pair produces three artifacts:
   - A one-paragraph description of the dominant constraint.
   - A recommended virtualization technology (specific product name allowed).
   - A diagram of the stack from hardware up — exactly the kind of diagram Assignment 5 wants and Project 7 will showcase.

3. **Present (3 min).** Each workload's pair presents their recommendation. Cross-comparison: same product, different workloads — does the recommendation differ?

### Discussion prompts
- For W2 (CI/CD), why might containers win even though microVMs offer stronger isolation?
- For W3 (medical records), what additional question would you ask the customer that the syllabus doesn't list? (Hint: where do their cryptographic keys live?)
- Assignment 5 requires you to discuss stewardship and ethical considerations in your final write-up. For W3, sketch one ethical question the architecture forces you to confront.

### Instructor notes
- Common pitfall: students pick a technology by familiarity rather than fit. Push them: "What constraint forces this choice?"
- Expected outcomes: W1 → Type-1 hypervisor (KVM/ESXi). W2 → containers + Kubernetes. W3 → confidential VMs (AMD SEV, Intel TDX) or microVMs with strong tenant separation.
- Time checkpoint: by minute 12 every pair should have a stack diagram and one-paragraph justification.

### Stretch goal
For your assigned workload, sketch how disaster recovery works. What gets backed up? What's the RTO budget? Where does the backup live (same region? different region?). This is one of the criteria Assignment 5 scores you on.

---

## Activity 2 — Containerize your shell

**Duration:** ~40 minutes (5 min frame · 25 min code · 10 min compare)
**Format:** Individual at laptop; pairs allowed for debugging
**Aligned objectives:** 2, 3, 4

### Bridge to assignment
Project 7 requires you to demo your shell. A containerized shell is portable, reproducible, and impressive — the demo runs the same on your machine, the instructor's machine, and a recording rig. This activity also exercises the container-vs-VM trade-off (boot time) you'll write about in Assignment 5.

### Setup
- Working Linux/UNIX environment with Docker installed (`docker --version` should work).
- Your shell project source (or use the Topic 2 `lopeShell.c` as a stand-in).
- A new directory: `~/CST-315/Topic8`.

### Steps

#### Part A — A minimal Dockerfile (5 min)
Discuss together: a Dockerfile is a script the container engine uses to build an image — a frozen filesystem with an entrypoint command.

#### Part B — Build the image (25 min)

1. Copy your shell source into `~/CST-315/Topic8/`. If you don't have your shell ready, use `lopeShell.c` from Topic 2.

2. Create `~/CST-315/Topic8/Dockerfile`:

```dockerfile
# Dockerfile for the CST-315 shell project
FROM debian:bookworm-slim

# Install build tools
RUN apt-get update && apt-get install -y --no-install-recommends \
        gcc libc6-dev make \
    && rm -rf /var/lib/apt/lists/*

# Copy source and build
WORKDIR /opt/shell
COPY lopeShell.c .
RUN gcc lopeShell.c -o lopeShell

# Default to running the shell interactively
CMD ["./lopeShell"]
```

3. Build the image:

```bash
docker build -t lopeshell:demo .
```

4. Run the shell in a container:

```bash
docker run --rm -it lopeshell:demo
```

You should land at your `lopeShell>` prompt inside the container. Try `ls`, `pwd`, `whoami` — note the container's filesystem and identity, not yours.

5. Time the boot:

```bash
time docker run --rm lopeshell:demo /bin/true
```

Record the real time.

#### Part C — Compare to a VM (10 min)
On the board, the instructor projects estimated boot times for the same payload:

- Bare-metal program start: ~1 ms
- Container start (`docker run`): ~200–800 ms
- microVM start (Firecracker): ~125 ms (faster than Docker; surprisingly)
- Traditional VM start (VirtualBox/ESXi): 5–30 s
- Cold-boot a full Linux VM: 1+ min

Discuss why a microVM beats a container on start time despite having stronger isolation. (Hint: container start is dominated by image extraction and engine overhead; Firecracker is minimal.)

Commit your `Dockerfile` to `~/CST-315/Topic8/` and push.

### Discussion prompts
- The Dockerfile uses `debian:bookworm-slim`. What's in that base image? What changes if you switch to `alpine:3.18`?
- Your container shares the host kernel. What does that mean for a security audit on a multi-tenant cluster running this image?
- For Project 7, would you record your demo from inside a container or from a VM? Defend your choice in one paragraph.

### Instructor notes
- Common pitfalls: forgetting `-it` (interactive + TTY), so the shell exits immediately; building on Apple Silicon and not adding `--platform linux/amd64` for cross-arch demonstration.
- Expected outcomes: every student gets a containerized shell. The boot-time comparison is the conceptual win — they should walk away with a felt sense of the latency budget across the virtualization spectrum.
- Time checkpoint: by minute 30 every student should have `docker run --rm -it lopeshell:demo` working.

### Stretch goal
Add a second stage to the Dockerfile that builds the binary in one image and copies just the binary into a tiny final image (e.g., `FROM scratch` if you statically link, or `FROM debian:bookworm-slim` without the build tools). Compare the final image size with `docker images`. This is one of Assignment 5's "image size" considerations made concrete.

---

## Closeout (last 2 minutes of class — end of term)

Direct students to:
- Finalize Project 7's screencast. Demonstrate the shell, walk through diagrams, and reference the master factory visualization throughout — anchoring your design choices in a shared mental model is exactly what good technical communication looks like.
- Finish Assignment 5's product survey. Use the architecture you sketched in Activity 1 as the workload context.
- Submit the Topics 7–8 quiz before the close of the term.
- Bring questions to office hours during the final week. You built a UNIX shell from the ground up over seven weeks. That's something to be proud of.
