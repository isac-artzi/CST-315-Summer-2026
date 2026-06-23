# Topic 6 — In-Class Activities

**Course:** CST-315 Operating Systems Lecture & Lab
**Topic:** I/O and File Management
**Reading:** Stallings, Chapters 11–12
**Companion lesson:** [Topic_6_IO_File_Management.html](../lessons/Topic_6_IO_File_Management.html)
**Master visualization:** [Factory_OS_Master.html](../Factory_OS_Master.html)

Activity 1 has students design directory-entry data structures and weigh allocation methods on the whiteboard. Activity 2 implements `mkdir` and `rmdir` in C as the seed of Project 6.

---

## Activity 1 — Design a directory entry and pick an allocation scheme

**Duration:** ~20 minutes (5 min framing · 12 min work · 3 min debrief)
**Format:** Pairs, on paper or whiteboard
**Aligned objectives:** 1, 2, 3

### Bridge to assignment
Project 6 requires 13 distinct file/directory commands. None of them can be implemented until the data structures are right. This activity gets the data structures explicit before any C is written.

### Setup
- Whiteboard or paper.
- A sample directory state: a root with two subdirectories and three files.

### Steps
1. **Frame (5 min).** Confirm the constraints from the Project 6 spec: hierarchical directory structure; per-file basic and detailed info; per-directory basic and detailed info; non-empty deletion must require a flag; storage must not overlap with Project 4's swap area.

2. **Design (12 min).** Each pair produces three artifacts on the whiteboard:
   - **Directory entry struct.** Sketch the fields needed for one entry: name, inode/handle, type (file vs dir), pointer to children if directory, parent pointer or path.
   - **File inode struct.** Sketch fields: inode number, size, owner, permissions, timestamps, allocation pointers (which scheme?).
   - **Allocation table.** Pick one allocation method (contiguous, linked, or indexed) and write a one-sentence defense.

3. **Debrief (3 min).** Two pairs present. Compare allocation choices. Which scheme is most defensible for a course project where files can be small or large and the disk isn't gigantic?

### Discussion prompts
- Why include a parent pointer in directory entries? When would you need it?
- The Project 6 spec asks for a flag-based detailed view (`info -d`). Which fields go in basic vs detailed?
- Your scheme allocates the first block at disk offset 0. Project 4's pager wants 0 too. Reconcile.

### Instructor notes
- Common pitfall: students sketch a directory entry that holds the file's contents inline. Push back — the entry holds metadata, the contents live elsewhere.
- Expected outcomes: indexed allocation is the right answer for this project (it supports both small and large files and random access). Students who pick contiguous should acknowledge external fragmentation as a deal-breaker.
- Time checkpoint: by minute 12 every pair should be presenting allocation choices.

### Stretch goal
Sketch how your data structures would represent symbolic and hard links. Why is a hard link "an extra directory entry pointing to the same inode" while a symbolic link is "a small file whose content is a path"?

---

## Activity 2 — Implement mkdir and rmdir in your shell

**Duration:** ~40 minutes (5 min frame · 30 min code · 5 min discuss)
**Format:** Individual at laptop; pairs allowed for debugging
**Aligned objectives:** 1, 2, 3, 4

### Bridge to assignment
This activity produces the two foundational directory operations of Project 6, plus the in-memory directory tree they operate on. Adding the remaining 11 commands (`touch`, `rm`, `mv`, `cp`, `cp -r`, search, tree, info, etc.) becomes a fill-in-the-pattern exercise.

### Setup
- Working Linux/UNIX environment.
- A new directory: `~/CST-315/Topic6`.

### Steps

#### Part A — Data structures (5 min)
Define on the board:

```c
typedef struct Node {
    char name[64];
    int  is_dir;
    struct Node *parent;
    struct Node *children;   /* linked list head, NULL for files */
    struct Node *next;       /* sibling pointer */
    int  inode;
} Node;
```

Justify: linked list of children means O(N) lookups within a directory, but is easy to implement and fine for small directories. Project 6 may justify a tree or hash map for larger directories.

#### Part B — Implement (30 min)

Create `~/CST-315/Topic6/fs.c`:

```c
/* fs.c — minimal in-memory file system: mkdir, rmdir, ls, pwd */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char name[64];
    int  is_dir;
    struct Node *parent;
    struct Node *children;
    struct Node *next;
    int  inode;
} Node;

static int next_inode = 1;
static Node *root = NULL;
static Node *cwd  = NULL;

Node *make_node(const char *name, int is_dir) {
    Node *n = calloc(1, sizeof(Node));
    strncpy(n->name, name, sizeof(n->name)-1);
    n->is_dir = is_dir;
    n->inode  = next_inode++;
    return n;
}
void add_child(Node *parent, Node *child) {
    child->parent = parent;
    child->next = parent->children;
    parent->children = child;
}
Node *find_child(Node *parent, const char *name) {
    for (Node *c = parent->children; c; c = c->next)
        if (strcmp(c->name, name) == 0) return c;
    return NULL;
}
int count_children(Node *parent) {
    int n = 0;
    for (Node *c = parent->children; c; c = c->next) n++;
    return n;
}

int do_mkdir(const char *name) {
    if (find_child(cwd, name)) {
        printf("mkdir: %s: already exists\n", name);
        return -1;
    }
    add_child(cwd, make_node(name, 1));
    return 0;
}

int do_rmdir(const char *name, int force) {
    Node *target = find_child(cwd, name);
    if (!target)              { printf("rmdir: %s: not found\n", name); return -1; }
    if (!target->is_dir)      { printf("rmdir: %s: not a directory\n", name); return -1; }
    if (count_children(target) > 0 && !force) {
        printf("rmdir: %s: not empty (use -r)\n", name); return -1;
    }
    /* unlink from sibling list */
    Node **pp = &cwd->children;
    while (*pp && *pp != target) pp = &(*pp)->next;
    if (*pp) *pp = target->next;
    /* recursive free */
    if (force) {
        Node *c = target->children;
        while (c) { Node *n = c->next; free(c); c = n; }
    }
    free(target);
    return 0;
}

void do_ls(void) {
    for (Node *c = cwd->children; c; c = c->next)
        printf("%s%s  ", c->name, c->is_dir ? "/" : "");
    printf("\n");
}
void do_pwd(void) {
    char buf[1024] = ""; char tmp[1024];
    Node *n = cwd;
    while (n && n->parent) {
        snprintf(tmp, sizeof(tmp), "/%s%s", n->name, buf);
        strncpy(buf, tmp, sizeof(buf));
        n = n->parent;
    }
    printf("%s\n", buf[0] ? buf : "/");
}

int main(void) {
    root = make_node("/", 1);
    cwd  = root;
    char line[256];
    printf("fs> ");
    while (fgets(line, sizeof(line), stdin)) {
        line[strcspn(line, "\n")] = 0;
        char *cmd = strtok(line, " ");
        char *a1  = strtok(NULL, " ");
        char *a2  = strtok(NULL, " ");
        if (!cmd) { ; }
        else if (strcmp(cmd, "mkdir") == 0 && a1) do_mkdir(a1);
        else if (strcmp(cmd, "rmdir") == 0 && a1) {
            int force = a2 && strcmp(a2, "-r") == 0;
            do_rmdir(a1, force);
        }
        else if (strcmp(cmd, "ls") == 0) do_ls();
        else if (strcmp(cmd, "pwd") == 0) do_pwd();
        else if (strcmp(cmd, "exit") == 0) break;
        else printf("unknown: %s\n", cmd);
        printf("fs> ");
    }
    return 0;
}
```

Compile and run:

```bash
gcc fs.c -o fs
./fs
```

Try:
```
fs> mkdir docs
fs> mkdir src
fs> ls
fs> rmdir docs
fs> ls
fs> mkdir docs
fs> mkdir docs/note
```
The last command should fail — your `mkdir` only looks at `cwd`. Note this for Project 6: you'll need to extend it to parse paths.

Commit and push.

### Discussion prompts
- Your `rmdir -r` recursively frees children but doesn't recurse into grand-children. Where is that bug?
- The inode counter is shared with files (when you add `touch`). Why is that correct?
- Project 4's pager and your fs both allocate memory on the heap. They share the same heap — but they cannot overlap. How do you ensure that in your code?

### Instructor notes
- Common pitfalls: not zeroing the Node struct (calloc fixes); using `strncpy` without ensuring null-termination; forgetting to update the sibling list when removing a node.
- Expected outcomes: students leave with a working in-memory tree they can operate on at the prompt. The bug in recursive rmdir is intentional — it appears in their Project 6 unless they explicitly fix it.
- Time checkpoint: by minute 30 every student should be at the prompt running mkdir/rmdir.

### Stretch goal
Add `touch <name>` (creates a file) and `tree` (recursively prints the directory tree with indentation showing nesting). Both are required by Project 6.

---

## Closeout (last 2 minutes of class)

Direct students to:
- Begin Project 6's allocation-scheme decision tonight. The whiteboard work from Activity 1 should make this a writing exercise rather than a thinking exercise.
- Verify the disk regions you'll use for files don't overlap Project 4's swap. Document the layout in your project README.
- Read Stallings Ch. 13 before next class — Topic 7 covers embedded systems and security.
