# Topic 6 · Starter — `fs`

A skeleton in-memory file system. The REPL, node primitives, path splitter, and most command implementations are in place. Three TODOs are marked: the path resolver, the rm/rmdir empty check, and the recursive tree print.

## What you have to do

Open `src/fs.c`. Three TODOs are marked:

1. **`resolve()`** — walk a path string from a starting node, returning the final node or NULL. Handle `/` (absolute), `.`, and `..`.
2. **`cmd_rm_or_rmdir()`** — refuse to remove a non-empty directory unless `-r` is supplied; otherwise unlink and free.
3. **`tree_recurse()`** — print the subtree under a node, indented by depth.

## Build, run, test
```bash
make
./fs
make test
```

## Hints
- `strtok(buf, "/")` splits on slashes. Watch out: it can't be interleaved with another strtok call.
- For `..`, just walk `cur->parent`; if cur is root, stay at root.
- For recursive remove, the helper `free_subtree(target)` already exists; just call it after `unlink_node`.

## Bridges to
- Lesson: `lessons/Topic_6_IO_File_Management.html`
- Project 6 — File System Manager
