/*
 * stat_demo.c — CST-315 · Topic 6 · demos · INSTRUCTOR ONLY
 * Purpose: Call stat() and dump the inode, size, mode, timestamps,
 *          and link count. Shows what an inode IS.
 * Build:   make stat_demo
 * Run:     ./stat_demo <path>
 * Notes:   Portable POSIX.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

int main(int argc, char **argv) {
    if (argc < 2) { fprintf(stderr, "usage: %s <path>\n", argv[0]); return 1; }
    struct stat st;
    if (stat(argv[1], &st) != 0) { perror(argv[1]); return 1; }

    printf("path:      %s\n", argv[1]);
    printf("inode:     %llu\n", (unsigned long long)st.st_ino);
    printf("size:      %lld bytes\n", (long long)st.st_size);
    printf("links:     %llu\n", (unsigned long long)st.st_nlink);
    printf("mode:      %o (octal)\n", (unsigned)(st.st_mode & 07777));
    printf("uid/gid:   %u/%u\n", (unsigned)st.st_uid, (unsigned)st.st_gid);
    printf("type:      %s\n",
           S_ISREG(st.st_mode) ? "regular file" :
           S_ISDIR(st.st_mode) ? "directory" :
           S_ISLNK(st.st_mode) ? "symlink" :
           S_ISBLK(st.st_mode) ? "block device" :
           S_ISCHR(st.st_mode) ? "char device" :
           S_ISFIFO(st.st_mode) ? "fifo" : "other");
    char buf[64];
    struct tm *tm = localtime(&st.st_mtime);
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm);
    printf("modified:  %s\n", buf);
    return 0;
}
