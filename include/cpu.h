#pragma once

#include "exec/cpu-defs.h"
#include "exec/cpu-all.h"

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

#define PAGE_READ 1
#define PAGE_WRITE 2
#define PAGE_EXEC 4
#define PAGE_BITS 7
#define PAGE_VALID 8

typedef struct CPUState {
	void *opaque;
	uint64_t random_seed;
} CPUState;

CPUState *env_cpu(CPUArchState *env);

// unused
void *first_cpu;
bool parallel_cpus;
#define CPU_NEXT(x) (0)
#define cpu_copy(x) (x)
