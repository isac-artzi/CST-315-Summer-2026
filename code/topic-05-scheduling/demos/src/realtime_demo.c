/*
 * realtime_demo.c — CST-315 · Topic 5 · demos · INSTRUCTOR ONLY
 * Purpose: Promote the calling thread to SCHED_FIFO with high
 *          priority. Print the before/after policy and priority.
 *          May require root or CAP_SYS_NICE to set policy.
 * Build:   make realtime_demo
 * Run:     ./realtime_demo            (may need sudo)
 * Notes:   Linux only. On macOS prints a friendly skip message.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __linux__

#include <pthread.h>
#include <sched.h>

static const char *policy_name(int p) {
    switch (p) {
        case SCHED_OTHER: return "SCHED_OTHER";
        case SCHED_FIFO:  return "SCHED_FIFO";
        case SCHED_RR:    return "SCHED_RR";
        default:          return "?";
    }
}

int main(void) {
    int policy;
    struct sched_param sp;
    pthread_getschedparam(pthread_self(), &policy, &sp);
    printf("before: policy=%s priority=%d\n", policy_name(policy), sp.sched_priority);

    sp.sched_priority = 50;
    int rc = pthread_setschedparam(pthread_self(), SCHED_FIFO, &sp);
    if (rc != 0) {
        fprintf(stderr, "setschedparam: %s\n", strerror(rc));
        fprintf(stderr, "(try running with sudo, or set RTPRIO via 'ulimit -r'.)\n");
        return 1;
    }
    pthread_getschedparam(pthread_self(), &policy, &sp);
    printf("after:  policy=%s priority=%d\n", policy_name(policy), sp.sched_priority);
    return 0;
}

#else  /* !__linux__ */

int main(void) {
    printf("realtime_demo: this demo is Linux-only.\n");
    printf("On macOS, see 'taskpolicy' and Mach thread priority APIs.\n");
    return 0;
}

#endif
