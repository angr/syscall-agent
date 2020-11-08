#pragma once

// copied in parts from qemu/include/qemu/log.h and qemu/include/qemu/log-for-trace.h
extern int qemu_loglevel;

/* Returns true if a bit is set in the current loglevel mask */
static inline bool qemu_loglevel_mask(int mask)
{
    return (qemu_loglevel & mask) != 0;
}

#define qemu_log_mask(MASK, FMT, ...)                   \
    do {                                                \
        if (unlikely(qemu_loglevel_mask(MASK))) {       \
            qemu_log(FMT, ## __VA_ARGS__);              \
        }                                               \
    } while (0)


/* main logging function */
int GCC_FMT_ATTR(1, 2) qemu_log(const char *fmt, ...);

#define LOG_STRACE 1
#define LOG_UNIMP 2
