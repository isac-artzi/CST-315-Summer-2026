# Topic 6 · Activity 1 — Design a Directory Entry and Pick an Allocation Scheme

**Course:** CST-315 Operating Systems Lecture & Lab
**Duration:** ~20 minutes
**Format:** Pairs, on paper or whiteboard

## Aligned objectives
1. Implement a file system with a hierarchical directory structure.
2. Manage the file life cycle.
3. Manage the directory life cycle.

## What you will produce
Three whiteboard artifacts:
1. A C struct sketch for a directory entry.
2. A C struct sketch for a file inode.
3. A choice of allocation scheme (contiguous, linked, or indexed) with a one-sentence defense.

## Setup
- Whiteboard or paper.
- A sample directory state: root with two subdirectories and three files.

## Steps

1. **Frame (5 min).** Recap Project 6's constraints: hierarchical structure; per-file basic and detailed info; per-directory basic and detailed info; non-empty deletion requires a flag; storage must not overlap with Project 4's swap area.

2. **Design (12 min).** With your partner, produce on the whiteboard:
   - **Directory entry struct.** Fields needed for one entry: name, inode/handle, type (file vs directory), pointer to children if directory, parent pointer or path.
   - **File inode struct.** Fields: inode number, size, owner, permissions, timestamps, allocation pointers.
   - **Allocation table.** Pick one scheme (contiguous, linked, or indexed) and write one sentence defending it.

3. **Present (3 min).** Two pairs share. Compare allocation choices.

## Discussion prompts
- Why include a parent pointer in directory entries? When would you need it?
- The Project 6 spec asks for a flag-based detailed view (`info -d`). Which fields go in basic vs detailed?
- Your scheme allocates the first block at disk offset 0. Project 4's pager wants 0 too. Reconcile.

## Stretch goal
Sketch how your data structures would represent symbolic and hard links. Why is a hard link "an extra directory entry pointing to the same inode" while a symbolic link is "a small file whose content is a path"?

---
**Companion lesson:** [Topic_6_IO_File_Management.html](../lessons/Topic_6_IO_File_Management.html)
**Master visualization:** [Factory_OS_Master.html](../Factory_OS_Master.html)
