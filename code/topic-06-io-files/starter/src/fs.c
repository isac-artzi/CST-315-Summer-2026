/*
 * fs.c — CST-315 · Topic 6 · starter
 * Purpose: In-memory hierarchical file system with a REPL.
 *
 * STUDENT TODO: complete resolve(), cmd_rm_or_rmdir(), tree_recurse().
 *
 * The REPL loop, node primitives, path splitter, and many commands
 * (mkdir, touch, ls, cd, pwd, info, find, mv, cp) are provided as
 * reference. You may replace any of them with your own version.
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
    int   size;
    struct Node *parent;
    struct Node *children;
    struct Node *next;
} Node;

static int   next_inode = 1;
static Node *root = NULL;
static Node *cwd  = NULL;

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

/* ===========================================================
 * TODO Topic 6 (1 of 3): resolve a path.
 *
 * Walk `path` token-by-token, starting from `start` (or from root
 * if the path begins with '/'). Handle "." (no-op) and ".." (parent;
 * stay at root if already there). Return the final node, or NULL if
 * any non-existent component is encountered.
 *
 * Hint: strtok(buf, "/") for tokenizing.
 * Hint: copy path into a local buffer first — strtok modifies it.
 * Hint: if you hit a non-directory mid-path, return NULL.
 * =========================================================== */
static Node *resolve(Node *start, const char *path) {
    /* TODO: replace this stub */
    (void)path;
    return start;
}

static void split_path(const char *path, char *parent_out, char *leaf_out) {
    const char *slash = strrchr(path, '/');
    if (!slash) { strcpy(parent_out, "."); strcpy(leaf_out, path); }
    else if (slash == path) { strcpy(parent_out, "/"); strcpy(leaf_out, slash + 1); }
    else {
        size_t pl = (size_t)(slash - path);
        memcpy(parent_out, path, pl); parent_out[pl] = '\0';
        strcpy(leaf_out, slash + 1);
    }
}
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
    if (find_child(parent, leaf))   return 0;
    add_child(parent, make_node(leaf, 0));
    return 0;
}

/* ===========================================================
 * TODO Topic 6 (2 of 3): rm and rmdir.
 *
 * Resolve the parent and find the target child.
 *   - If !target: print "not found".
 *   - If dir_only and target is not a dir: print "not a directory".
 *   - If target is a non-empty dir AND !recursive: print "not empty (use -r)".
 *   - Otherwise unlink_node(parent, target) and free_subtree(target).
 * Return 0 on success, -1 on error.
 * =========================================================== */
static int cmd_rm_or_rmdir(const char *path, int dir_only, int recursive) {
    /* TODO: replace this stub */
    (void)path; (void)dir_only; (void)recursive;
    printf("rm: TODO: not yet implemented\n");
    return -1;
}

static void cmd_ls(const char *path) {
    Node *t = path ? resolve(cwd, path) : cwd;
    if (!t) { printf("ls: %s: not found\n", path); return; }
    if (!t->is_dir) { printf("%s\n", t->name); return; }
    for (Node *c = t->children; c; c = c->next)
        printf("%s%s  ", c->name, c->is_dir ? "/" : "");
    printf("\n");
}

/* ===========================================================
 * TODO Topic 6 (3 of 3): recursive tree printer.
 *
 * Print "  " repeated `depth` times, then n->name (with trailing
 * '/' if it's a directory), then a newline. Then recurse over
 * every child at depth + 1.
 * =========================================================== */
static void tree_recurse(Node *n, int depth) {
    /* TODO: replace this stub */
    (void)n; (void)depth;
}

static void cmd_tree(const char *path) {
    Node *t = path ? resolve(cwd, path) : cwd;
    if (!t) { printf("tree: %s: not found\n", path); return; }
    tree_recurse(t, 0);
}
static int cmd_cd(const char *path) {
    Node *t = path ? resolve(cwd, path) : root;
    if (!t) { printf("cd: %s: not found\n", path); return -1; }
    if (!t->is_dir) { printf("cd: %s: not a directory\n", path); return -1; }
    cwd = t;
    return 0;
}
static void cmd_pwd(void) {
    char buf[LINE_MAX_LEN]; build_pwd(cwd, buf, sizeof(buf));
    printf("%s\n", buf);
}

static char *trim(char *s) {
    while (*s && isspace((unsigned char)*s)) s++;
    char *e = s + strlen(s);
    while (e > s && isspace((unsigned char)e[-1])) *--e = '\0';
    return s;
}

int main(void) {
    root = make_node("/", 1);
    cwd  = root;
    char line[LINE_MAX_LEN], buf[LINE_MAX_LEN];
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
        else printf("unknown: %s\n", cmd);
    }
    free_subtree(root);
    return 0;
}
