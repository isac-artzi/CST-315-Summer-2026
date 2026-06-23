/*
 * fs.c — CST-315 · Topic 6 · milestone
 * Purpose: In-memory hierarchical file system with a REPL. Core
 *          commands: mkdir, rmdir [-r], touch, rm, ls, tree, cd,
 *          pwd, info, find, mv, cp, exit.
 * Build:   make
 * Run:     ./fs
 * Notes:   POSIX. Builds on macOS and Linux.
 *          Project 6 extends this with detailed-info flags and
 *          additional refinements — drop them on top of this.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NAME_MAX_LEN 64
#define LINE_MAX_LEN 1024

typedef struct Node {
    char  name[NAME_MAX_LEN];
    int   is_dir;
    int   inode;
    int   size;                 /* file content "size" (just a counter here) */
    struct Node *parent;
    struct Node *children;      /* linked list head (files & dirs)           */
    struct Node *next;          /* sibling pointer                           */
} Node;

static int   next_inode = 1;
static Node *root = NULL;
static Node *cwd  = NULL;

/* ------- tree primitives ------- */
static Node *make_node(const char *name, int is_dir) {
    Node *n = calloc(1, sizeof(Node));
    strncpy(n->name, name, NAME_MAX_LEN - 1);
    n->is_dir = is_dir;
    n->inode  = next_inode++;
    return n;
}
static void add_child(Node *parent, Node *child) {
    child->parent = parent;
    child->next = parent->children;
    parent->children = child;
}
static Node *find_child(Node *parent, const char *name) {
    for (Node *c = parent->children; c; c = c->next)
        if (strcmp(c->name, name) == 0) return c;
    return NULL;
}
static int count_children(Node *parent) {
    int n = 0;
    for (Node *c = parent->children; c; c = c->next) n++;
    return n;
}
static void unlink_node(Node *parent, Node *target) {
    Node **pp = &parent->children;
    while (*pp && *pp != target) pp = &(*pp)->next;
    if (*pp) *pp = target->next;
}
static void free_subtree(Node *n) {
    Node *c = n->children;
    while (c) { Node *nx = c->next; free_subtree(c); c = nx; }
    free(n);
}

/* ------- path resolution -------
 * Resolve a path string relative to `start`.
 * Returns the node, or NULL if any component is missing. */
static Node *resolve(Node *start, const char *path) {
    Node *cur = (path[0] == '/') ? root : start;
    char buf[LINE_MAX_LEN];
    strncpy(buf, path, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';
    char *tok = strtok(buf, "/");
    while (tok) {
        if (strcmp(tok, ".") == 0) {
            /* stay */
        } else if (strcmp(tok, "..") == 0) {
            if (cur->parent) cur = cur->parent;
        } else {
            if (!cur->is_dir) return NULL;
            Node *ch = find_child(cur, tok);
            if (!ch) return NULL;
            cur = ch;
        }
        tok = strtok(NULL, "/");
    }
    return cur;
}

/* Split a path into (parent_path, leaf_name). Both buffers
 * pre-allocated by the caller. */
static void split_path(const char *path, char *parent_out, char *leaf_out) {
    const char *slash = strrchr(path, '/');
    if (!slash) {
        strcpy(parent_out, ".");
        strcpy(leaf_out, path);
    } else if (slash == path) {
        strcpy(parent_out, "/");
        strcpy(leaf_out, slash + 1);
    } else {
        size_t pl = (size_t)(slash - path);
        memcpy(parent_out, path, pl);
        parent_out[pl] = '\0';
        strcpy(leaf_out, slash + 1);
    }
}

/* ------- pwd buffer ------- */
static void build_pwd(Node *n, char *out, size_t cap) {
    if (n == root) { strncpy(out, "/", cap); return; }
    char buf[LINE_MAX_LEN] = "";
    char tmp[LINE_MAX_LEN];
    Node *cur = n;
    while (cur && cur != root) {
        snprintf(tmp, sizeof(tmp), "/%s%s", cur->name, buf);
        strncpy(buf, tmp, sizeof(buf) - 1);
        cur = cur->parent;
    }
    strncpy(out, buf, cap);
}

/* ------- commands ------- */
static int cmd_mkdir(const char *path) {
    char p[LINE_MAX_LEN], leaf[NAME_MAX_LEN];
    split_path(path, p, leaf);
    Node *parent = resolve(cwd, p);
    if (!parent || !parent->is_dir) { printf("mkdir: %s: parent not found\n", path); return -1; }
    if (find_child(parent, leaf))   { printf("mkdir: %s: exists\n", path); return -1; }
    add_child(parent, make_node(leaf, 1));
    return 0;
}
static int cmd_touch(const char *path) {
    char p[LINE_MAX_LEN], leaf[NAME_MAX_LEN];
    split_path(path, p, leaf);
    Node *parent = resolve(cwd, p);
    if (!parent || !parent->is_dir) { printf("touch: %s: parent not found\n", path); return -1; }
    if (find_child(parent, leaf))   return 0;       /* already exists — no-op */
    add_child(parent, make_node(leaf, 0));
    return 0;
}
static int cmd_rm_or_rmdir(const char *path, int dir_only, int recursive) {
    char p[LINE_MAX_LEN], leaf[NAME_MAX_LEN];
    split_path(path, p, leaf);
    Node *parent = resolve(cwd, p);
    if (!parent) { printf("rm: %s: not found\n", path); return -1; }
    Node *target = find_child(parent, leaf);
    if (!target) { printf("rm: %s: not found\n", path); return -1; }
    if (dir_only && !target->is_dir) { printf("rmdir: %s: not a directory\n", path); return -1; }
    if (target->is_dir && count_children(target) > 0 && !recursive) {
        printf("rmdir: %s: not empty (use -r)\n", path); return -1;
    }
    unlink_node(parent, target);
    free_subtree(target);
    return 0;
}
static void cmd_ls(const char *path) {
    Node *t = path ? resolve(cwd, path) : cwd;
    if (!t) { printf("ls: %s: not found\n", path); return; }
    if (!t->is_dir) { printf("%s%s  ", t->name, ""); printf("\n"); return; }
    for (Node *c = t->children; c; c = c->next)
        printf("%s%s  ", c->name, c->is_dir ? "/" : "");
    printf("\n");
}
static void tree_recurse(Node *n, int depth) {
    for (int i = 0; i < depth; i++) printf("  ");
    printf("%s%s\n", n->name, n->is_dir ? "/" : "");
    for (Node *c = n->children; c; c = c->next) tree_recurse(c, depth + 1);
}
static void cmd_tree(const char *path) {
    Node *t = path ? resolve(cwd, path) : cwd;
    if (!t) { printf("tree: %s: not found\n", path); return; }
    tree_recurse(t, 0);
}
static int cmd_cd(const char *path) {
    Node *t = path ? resolve(cwd, path) : root;
    if (!t)            { printf("cd: %s: not found\n", path); return -1; }
    if (!t->is_dir)    { printf("cd: %s: not a directory\n", path); return -1; }
    cwd = t;
    return 0;
}
static void cmd_pwd(void) {
    char buf[LINE_MAX_LEN]; build_pwd(cwd, buf, sizeof(buf));
    printf("%s\n", buf);
}
static void cmd_info(const char *path) {
    Node *t = resolve(cwd, path);
    if (!t) { printf("info: %s: not found\n", path); return; }
    printf("name:     %s\n", t->name);
    printf("type:     %s\n", t->is_dir ? "directory" : "file");
    printf("inode:    %d\n", t->inode);
    if (t->is_dir) printf("children: %d\n", count_children(t));
    else           printf("size:     %d bytes\n", t->size);
}
static void find_recurse(Node *n, const char *name, const char *prefix) {
    char path[LINE_MAX_LEN];
    snprintf(path, sizeof(path), "%s/%s", prefix[0] ? prefix : "", n->name);
    if (strcmp(n->name, name) == 0) printf("%s\n", path);
    for (Node *c = n->children; c; c = c->next) find_recurse(c, name, path);
}
static void cmd_find(const char *name, const char *from) {
    Node *start = (from && from[0]) ? resolve(cwd, from) : cwd;
    if (!start) { printf("find: %s: not found\n", from); return; }
    char base[LINE_MAX_LEN]; build_pwd(start, base, sizeof(base));
    if (strcmp(base, "/") == 0) base[0] = '\0';
    for (Node *c = start->children; c; c = c->next) find_recurse(c, name, base);
}
static int cmd_mv(const char *src, const char *dst) {
    char ps[LINE_MAX_LEN], ls[NAME_MAX_LEN], pd[LINE_MAX_LEN], ld[NAME_MAX_LEN];
    split_path(src, ps, ls);
    split_path(dst, pd, ld);
    Node *psn = resolve(cwd, ps); Node *pdn = resolve(cwd, pd);
    if (!psn || !pdn) { printf("mv: source or dest parent not found\n"); return -1; }
    Node *t = find_child(psn, ls);
    if (!t) { printf("mv: %s: not found\n", src); return -1; }
    if (find_child(pdn, ld)) { printf("mv: %s: exists\n", dst); return -1; }
    unlink_node(psn, t);
    strncpy(t->name, ld, NAME_MAX_LEN - 1);
    add_child(pdn, t);
    return 0;
}
static Node *deep_copy(Node *src) {
    Node *c = make_node(src->name, src->is_dir);
    c->size = src->size;
    for (Node *ch = src->children; ch; ch = ch->next) {
        Node *cc = deep_copy(ch);
        add_child(c, cc);
    }
    return c;
}
static int cmd_cp(const char *src, const char *dst) {
    Node *t = resolve(cwd, src);
    if (!t) { printf("cp: %s: not found\n", src); return -1; }
    char pd[LINE_MAX_LEN], ld[NAME_MAX_LEN];
    split_path(dst, pd, ld);
    Node *pdn = resolve(cwd, pd);
    if (!pdn) { printf("cp: dest parent not found\n"); return -1; }
    if (find_child(pdn, ld)) { printf("cp: %s: exists\n", dst); return -1; }
    Node *copy = deep_copy(t);
    strncpy(copy->name, ld, NAME_MAX_LEN - 1);
    add_child(pdn, copy);
    return 0;
}

/* ------- REPL ------- */
static char *trim(char *s) {
    while (*s && isspace((unsigned char)*s)) s++;
    char *e = s + strlen(s);
    while (e > s && isspace((unsigned char)e[-1])) *--e = '\0';
    return s;
}

int main(void) {
    root = make_node("/", 1);
    cwd  = root;

    char line[LINE_MAX_LEN];
    char buf[LINE_MAX_LEN];
    while (1) {
        build_pwd(cwd, buf, sizeof(buf));
        printf("fs:%s> ", buf);
        fflush(stdout);
        if (!fgets(line, sizeof(line), stdin)) break;
        char *cmd = trim(line);
        if (cmd[0] == '\0') continue;

        char *space = strchr(cmd, ' ');
        char *args = NULL;
        if (space) { *space = '\0'; args = trim(space + 1); }

        if      (strcmp(cmd, "exit") == 0) break;
        else if (strcmp(cmd, "mkdir") == 0 && args) cmd_mkdir(args);
        else if (strcmp(cmd, "touch") == 0 && args) cmd_touch(args);
        else if (strcmp(cmd, "rm") == 0 && args) {
            int recursive = 0;
            if (strncmp(args, "-r ", 3) == 0) { recursive = 1; args += 3; args = trim(args); }
            cmd_rm_or_rmdir(args, 0, recursive);
        }
        else if (strcmp(cmd, "rmdir") == 0 && args) {
            int recursive = 0;
            if (strncmp(args, "-r ", 3) == 0) { recursive = 1; args += 3; args = trim(args); }
            cmd_rm_or_rmdir(args, 1, recursive);
        }
        else if (strcmp(cmd, "ls") == 0) cmd_ls(args);
        else if (strcmp(cmd, "tree") == 0) cmd_tree(args);
        else if (strcmp(cmd, "cd") == 0) cmd_cd(args);
        else if (strcmp(cmd, "pwd") == 0) cmd_pwd();
        else if (strcmp(cmd, "info") == 0 && args) cmd_info(args);
        else if (strcmp(cmd, "find") == 0 && args) {
            char *sp = strchr(args, ' ');
            char *from = NULL;
            if (sp) { *sp = '\0'; from = trim(sp + 1); }
            cmd_find(args, from);
        }
        else if (strcmp(cmd, "mv") == 0 && args) {
            char *sp = strchr(args, ' ');
            if (sp) { *sp = '\0'; cmd_mv(args, trim(sp + 1)); }
            else printf("mv: usage: mv <src> <dst>\n");
        }
        else if (strcmp(cmd, "cp") == 0 && args) {
            char *sp = strchr(args, ' ');
            if (sp) { *sp = '\0'; cmd_cp(args, trim(sp + 1)); }
            else printf("cp: usage: cp <src> <dst>\n");
        }
        else printf("unknown: %s\n", cmd);
    }
    free_subtree(root);
    return 0;
}
