# Topic 7 · Activity 1 — Find the Injection Vector

**Course:** CST-315 Operating Systems Lecture & Lab
**Duration:** ~20 minutes
**Format:** Pairs, on paper

## Aligned objectives
1. Design and implement a simple command injection virus.
3. Demonstrate the logic of the C program in relation to embedded systems and security.
4. Analyze the malware program in conjunction with the suitability of the approach to malware creation.

## What you will produce
For each of three C snippets: the trust boundary marked, one malicious input that exploits it, and a two-sentence fix.

## Setup
- Three short C snippets (below).
- Pen and paper.

## Steps

1. **Frame (5 min).** A command injection vulnerability exists wherever user-controlled data flows into something that executes a command without sanitization. Targets include `system()`, `popen()`, and `execvp` calls where `argv` was assembled from input.

2. **Identify (10 min).** For each snippet, write on your paper:
   - The trust boundary (where untrusted data becomes executed).
   - One malicious input that exploits it.
   - A two-sentence fix.

### Snippet A
```c
char cmd[256];
snprintf(cmd, sizeof(cmd), "grep %s /etc/services", user_input);
system(cmd);
```

### Snippet B
```c
char *argv[3];
argv[0] = "/bin/sh";
argv[1] = "-c";
argv[2] = user_query;     /* user_query came from a TCP socket */
execv("/bin/sh", argv);
```

### Snippet C
```c
char *argv[3];
argv[0] = "/bin/rm";
argv[1] = filename;       /* filename = user_input */
argv[2] = NULL;
execvp(argv[0], argv);
```

3. **Present (5 min).** Two pairs present one snippet each.

## Discussion prompts
- Snippet C looks safer than A and B. Is it? What is the residual risk?
- For each snippet, what defensive change closes the vulnerability without breaking the legitimate use case?
- Assignment 4 forbids `system()`. Why does the spec rule out the easy path?

## Stretch goal
Snippet B uses `execv` with `/bin/sh -c`. If you replace it with `system(user_query)`, is the behavior identical? Explain.

---
**Companion lesson:** [Topic_7_Embedded_Security.html](../lessons/Topic_7_Embedded_Security.html)
**Master visualization:** [Factory_OS_Master.html](../Factory_OS_Master.html)
