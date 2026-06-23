# Topic 6 · Demos — INSTRUCTOR ONLY

> **Do not distribute these programs to students.**

| Binary | What it demonstrates |
|---|---|
| `stat_demo` | Call `stat()` on a path and print the inode, size, timestamps, mode, link count. Shows what the OS knows about a file. |
| `fd_table_demo` | Open the same file twice via separate `open()` calls. The two FDs have independent offsets — observed by reading one byte from each. |
| `simple_raid0` | Stripe a byte stream across two output files. Shows the principle of RAID 0 in 40 lines. |
| `mmap_io` | Read a file two ways (`read()` vs `mmap()`), compare wall-clock time. Often mmap wins for sequential scans. |

## Build & run
```bash
make
./stat_demo /etc/hosts
./fd_table_demo
./simple_raid0
./mmap_io /etc/passwd
```
