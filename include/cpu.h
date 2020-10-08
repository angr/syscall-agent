#pragma once

#include "exec/cpu-defs.h"
#include "exec/cpu-all.h"

#if defined(TARGET_X86)
#define R_ESP 0
#define R_EIP 1
#define R_EAX 2
#define R_NUM 3

#define R_CS 0
#define R_DS 1
#define R_ES 2
#define R_FS 3
#define R_GS 4
#define R_SS 5
#define R_NUM_SEG 6

typedef struct SegmentCache {
    uint32_t selector;
    target_ulong base;
    uint32_t limit;
    uint32_t flags;
} SegmentCache;

typedef struct CPUX86State {
	target_ulong regs[R_NUM];
	SegmentCache segs[R_NUM_SEG], gdt, ldt;
} CPUX86State;

typedef CPUX86State CPUArchState;

#elif defined(TARGET_MIPS)

#if defined(HOST_WORDS_BIGENDIAN)
#  define FP_ENDIAN_IDX 1
#else
#  define FP_ENDIAN_IDX 0
#endif

#define MIPS_HFLAG_F64    0x00040 /* 64-bit FPU enabled                 */
#define MIPS_HFLAG_FRE   0x2000000 /* FRE enabled */

#define FCR0_FREP 29
#define FCR0_F64 22

#define CP0C5_FRE          8

#define CP0St_FR    26

#define MSA_WRLEN (128)

typedef union wr_t wr_t;
union wr_t {
    int8_t  b[MSA_WRLEN / 8];
    int16_t h[MSA_WRLEN / 16];
    int32_t w[MSA_WRLEN / 32];
    int64_t d[MSA_WRLEN / 64];
};

typedef union fpr_t fpr_t;
union fpr_t {
    float64  fd;   /* ieee double precision */
    float32  fs[2];/* ieee single precision */
    uint64_t d;    /* binary double fixed-point */
    uint32_t w[2]; /* binary single fixed-point */
/* FPU/MSA register mapping is not tested on big-endian hosts. */
    wr_t     wr;   /* vector data */
};

typedef struct TCState TCState;
struct TCState {
    target_ulong gpr[32];
    target_ulong PC;
    target_ulong CP0_UserLocal;
};

typedef struct CPUMIPSFPUContext CPUMIPSFPUContext;
struct CPUMIPSFPUContext {
    fpr_t fpr[32];
    uint32_t fcr0;
};

typedef struct CPUMIPState {
    TCState active_tc;
    CPUMIPSFPUContext active_fpu;
    int32_t CP0_Status;
    uint32_t CP0_Status_rw_bitmask; /* Read/write bits in CP0_Status */
    int32_t CP0_Config5;
    uint32_t hflags;    /* CPU State */
} CPUMIPSState;

typedef CPUMIPSState CPUArchState;
#endif

#define PAGE_READ 1
#define PAGE_WRITE 2
#define PAGE_EXEC 4
#define PAGE_BITS 7
#define PAGE_VALID 8

typedef struct CPUState {
	void *opaque;
	uint64_t random_seed;
	CPUArchState *env_ptr;
} CPUState;

CPUState *env_cpu(CPUArchState *env);

// unused
#include "panic.h"
void *first_cpu;
bool parallel_cpus;
#define CPU_NEXT(x) (PANIC_UNREACHED(), 0)
#define cpu_copy(x) (PANIC_UNREACHED(), x)
#define cpu_x86_load_seg(x, y, z) PANIC_UNREACHED()
#define rcu_register_thread() PANIC_UNREACHED()
#define tcg_register_thread() PANIC_UNREACHED()
#define rcu_unregister_thread() PANIC_UNREACHED()
#define qemu_guest_random_seed_thread_part2(x) PANIC_UNREACHED()
#define qemu_guest_random_seed_thread_part1() (PANIC_UNREACHED(), 0)
#define tb_flush(x) PANIC_UNREACHED()
#define object_property_set_bool(w, x, y, z) PANIC_UNREACHED()
#define object_unref(x) PANIC_UNREACHED()
#define OBJECT(x) (PANIC_UNREACHED(), x)
