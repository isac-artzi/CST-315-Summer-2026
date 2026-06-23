# common/

Shared headers and tiny utility code used across multiple topic milestones.

Right now this folder is empty. It will fill in as the shell grows:

- Topic 2: `shell_types.h` — `Process` struct, state enum, basic queue types used by the scheduler later.
- Topic 4: `pager_api.h` — interface the shell uses to invoke the pager.
- Topic 5: `scheduler_api.h` — interface for procs/priority commands.
- Topic 6: `fs_api.h` — interface for the in-memory file system.

Headers in `common/` are included by milestone and starter code in later topics. Demos do not include them unless noted.
