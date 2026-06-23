# Topic 6 · Activity 2 — Implement mkdir and rmdir in Your Shell

**Course:** CST-315 Operating Systems Lecture & Lab
**Duration:** ~40 minutes
**Format:** Individual at your laptop; pairs allowed for debugging

## Aligned objectives
1. Implement a file system with a hierarchical directory structure.
2. Manage the file life cycle.
3. Manage the directory life cycle.
4. Explain code snippets to prevent the shell from collapsing with data loss.

## What you will produce
A working in-memory file system kernel in C with `mkdir`, `rmdir`, `rmdir -r`, `ls`, and `pwd`. This becomes the foundation of Project 6's File System Manager.

## Setup
- Working Linux/UNIX environment.
- A new directory: `~/CST-315/Topic6`.

## Steps

### Part A — Data structures (5 min)

```c
typedef struct Node {
    char name[64];
    int  is_dir;
    struct Node *parent;
    struct Node *children;   /* linked list head, NULL for files */
    struct Node *next;       /* sibling */
    int  inode;
} Node;
```

### Part B — Implement (30 min)

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
    if (find_child(cwd, name)) { printf("mkdir: %s: already exists\n", name); return -1; }
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
    Node **pp = &cwd->children;
    while (*pp && *pp != target) pp = &(*pp)->next;
    if (*pp) *pp = target->next;
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
fs> mkdir docs/note     # this will fail — note it
```

Screenshot, commit, push to `~/CST-315/Topic6/`.

## Discussion prompts
- Your `rmdir -r` recursively frees children but doesn't recurse into grand-children. Where's the bug?
- The inode counter is shared with files (when you later add `touch`). Why is that correct?
- Project 4's pager and your file system both allocate memory on the heap. They share the same heap but cannot overlap. How do you ensure that?

## Stretch goal
Add `touch <name>` (creates a file) and `tree` (recursively prints the directory tree with indentation). Both are required by Project 6.

---
**Companion lesson:** [Topic_6_IO_File_Management.html](../lessons/Topic_6_IO_File_Management.html)
**Master visualization:** [Factory_OS_Master.html](../Factory_OS_Master.html)
