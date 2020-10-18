#pragma once

// copied in chunks from include/qemu/osdep.h
#include <errno.h>
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <inttypes.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ucontext.h>
#include <sys/socket.h>
#include <sys/sysmacros.h>
#include <sys/inotify.h>
#include <linux/version.h>
#include <linux/utsname.h>
#include <netinet/in.h>

#define GLIB_VERSION_2_48 GLIB_VERSION_2_26
#include "glib-compat.h"
#ifndef g_autoptr
// ???
#define g_autoptr(x) x*
#endif
#include "compiler.h"

#define ROUND_UP(value, multiple) ((value) + (multiple) - 1) & -(multiple)
#define DIV_ROUND_UP(value, multiple) (((value) + (multiple) - 1) / (multiple))
#define qemu_host_page_size 0x1000
#define HOST_PAGE_ALIGN(addr) (ROUND_UP((addr), qemu_host_page_size))

#define QEMU_IS_ARRAY(x) (!__builtin_types_compatible_p(typeof(x), \
                                                        typeof(&(x)[0])))
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) ((sizeof(x) / sizeof((x)[0])) + \
                       QEMU_BUILD_BUG_ON_ZERO(!QEMU_IS_ARRAY(x)))
#endif

# define MIN_CONST(a, b)                                        \
    __builtin_choose_expr(                                      \
        __builtin_constant_p(a) && __builtin_constant_p(b),     \
        (a) < (b) ? (a) : (b),                                  \
        ((void)0))
# define MAX_CONST(a, b)                                        \
    __builtin_choose_expr(                                      \
        __builtin_constant_p(a) && __builtin_constant_p(b),     \
        (a) > (b) ? (a) : (b),                                  \
        ((void)0))

// compiler abstraction

#ifndef __has_warning
#define __has_warning(x) 0
#endif

// environment abstraction

#ifndef O_LARGEFILE
#define O_LARGEFILE 0
#endif

#ifndef RTC_VL_READ
#define RTC_VL_READ -1
#endif
#ifndef RTC_VL_CLR
#define RTC_VL_CLR -1
#endif

#ifndef MAP_HUGETLB
#define MAP_HUGETLB 0
#endif

#if UINTPTR_MAX == UINT32_MAX
# define HOST_LONG_BITS 32
#elif UINTPTR_MAX == UINT64_MAX
# define HOST_LONG_BITS 64
#else
# error Unknown pointer size
#endif

#define FMT_pid "%d"

// no idea why this is necessary but strace.c needs it
#include "qemu/bitops.h"
