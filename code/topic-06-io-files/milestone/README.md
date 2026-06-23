# Topic 6 · Milestone — `fs`

## Goal
An in-memory hierarchical file system with a REPL. Supports the core commands required by Project 6.

## Build
```bash
make
make test
make clean
```

## Run
```bash
./fs
fs:/> mkdir docs
fs:/> mkdir src
fs:/> touch src/main.c
fs:/> ls
fs:/> tree
fs:/> cd src
fs:/src> pwd
fs:/src> rm main.c
fs:/src> cd ..
fs:/> rmdir docs
fs:/> exit
```

Commands implemented: `mkdir`, `rmdir [-r]`, `touch`, `rm`, `ls [path]`, `tree`, `cd path`, `pwd`, `info path`, `find name [from]`, `mv src dst`, `cp src dst`, `exit`.

## Files
- `src/fs.c` — tree, path resolution, REPL, command implementations.

## Bridges to
- Lesson: `lessons/Topic_6_IO_File_Management.html`
- Activity 2: `activities/Topic_6_Activity_2.md`
- Project 6 — File System Manager

## Notes
- Pure POSIX C99. Builds on macOS and Linux.
- Project 6 asks for detailed-info flags, directory-vs-file `info`, and additional refinements; build them on top of this skeleton.
