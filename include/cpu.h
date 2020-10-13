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

#elif defined(TARGET_ARM)

#define CPSR_V (1U << 28)
#define CPSR_C (1U << 29)
#define CPSR_Z (1U << 30)
#define CPSR_N (1U << 31)
#define CPSR_NZCV (CPSR_N | CPSR_Z | CPSR_C | CPSR_V)

#define HCR_VM        (1ULL << 0)
#define HCR_SWIO      (1ULL << 1)
#define HCR_PTW       (1ULL << 2)
#define HCR_FMO       (1ULL << 3)
#define HCR_IMO       (1ULL << 4)
#define HCR_AMO       (1ULL << 5)
#define HCR_VF        (1ULL << 6)
#define HCR_VI        (1ULL << 7)
#define HCR_VSE       (1ULL << 8)
#define HCR_FB        (1ULL << 9)
#define HCR_BSU_MASK  (3ULL << 10)
#define HCR_DC        (1ULL << 12)
#define HCR_TWI       (1ULL << 13)
#define HCR_TWE       (1ULL << 14)
#define HCR_TID0      (1ULL << 15)
#define HCR_TID1      (1ULL << 16)
#define HCR_TID2      (1ULL << 17)
#define HCR_TID3      (1ULL << 18)
#define HCR_TSC       (1ULL << 19)
#define HCR_TIDCP     (1ULL << 20)
#define HCR_TACR      (1ULL << 21)
#define HCR_TSW       (1ULL << 22)
#define HCR_TPCP      (1ULL << 23)
#define HCR_TPU       (1ULL << 24)
#define HCR_TTLB      (1ULL << 25)
#define HCR_TVM       (1ULL << 26)
#define HCR_TGE       (1ULL << 27)
#define HCR_TDZ       (1ULL << 28)
#define HCR_HCD       (1ULL << 29)
#define HCR_TRVM      (1ULL << 30)
#define HCR_RW        (1ULL << 31)
#define HCR_CD        (1ULL << 32)
#define HCR_ID        (1ULL << 33)
#define HCR_E2H       (1ULL << 34)
#define HCR_TLOR      (1ULL << 35)
#define HCR_TERR      (1ULL << 36)
#define HCR_TEA       (1ULL << 37)
#define HCR_MIOCNCE   (1ULL << 38)
/* RES0 bit 39 */
#define HCR_APK       (1ULL << 40)
#define HCR_API       (1ULL << 41)
#define HCR_NV        (1ULL << 42)
#define HCR_NV1       (1ULL << 43)
#define HCR_AT        (1ULL << 44)
#define HCR_NV2       (1ULL << 45)
#define HCR_FWB       (1ULL << 46)
#define HCR_FIEN      (1ULL << 47)
/* RES0 bit 48 */
#define HCR_TID4      (1ULL << 49)
#define HCR_TICAB     (1ULL << 50)
#define HCR_AMVOFFEN  (1ULL << 51)
#define HCR_TOCU      (1ULL << 52)
#define HCR_ENSCXT    (1ULL << 53)
#define HCR_TTLBIS    (1ULL << 54)
#define HCR_TTLBOS    (1ULL << 55)
#define HCR_ATA       (1ULL << 56)
#define HCR_DCT       (1ULL << 57)
#define HCR_TID5      (1ULL << 58)
#define HCR_TWEDEN    (1ULL << 59)
#define HCR_TWEDEL    MAKE_64BIT_MASK(60, 4)

#define SCR_NS                (1U << 0)
#define SCR_IRQ               (1U << 1)
#define SCR_FIQ               (1U << 2)
#define SCR_EA                (1U << 3)
#define SCR_FW                (1U << 4)
#define SCR_AW                (1U << 5)
#define SCR_NET               (1U << 6)
#define SCR_SMD               (1U << 7)
#define SCR_HCE               (1U << 8)
#define SCR_SIF               (1U << 9)
#define SCR_RW                (1U << 10)
#define SCR_ST                (1U << 11)
#define SCR_TWI               (1U << 12)
#define SCR_TWE               (1U << 13)
#define SCR_TLOR              (1U << 14)
#define SCR_TERR              (1U << 15)
#define SCR_APK               (1U << 16)
#define SCR_API               (1U << 17)
#define SCR_EEL2              (1U << 18)
#define SCR_EASE              (1U << 19)
#define SCR_NMEA              (1U << 20)
#define SCR_FIEN              (1U << 21)
#define SCR_ENSCXT            (1U << 25)
#define SCR_ATA               (1U << 26)

typedef struct {
    uint64_t raw_tcr;
    uint32_t mask;
    uint32_t base_mask;
} TCR;

typedef struct ARMGenericTimer {
    uint64_t cval; /* Timer CompareValue register */
    uint64_t ctl; /* Timer Control register */
} ARMGenericTimer;

#define GTIMER_PHYS     0
#define GTIMER_VIRT     1
#define GTIMER_HYP      2
#define GTIMER_SEC      3
#define GTIMER_HYPVIRT  4
#define NUM_GTIMERS     5

typedef enum CPSRWriteType {
    CPSRWriteByInstr = 0,         /* from guest MSR or CPS */
    CPSRWriteExceptionReturn = 1, /* from guest exception return insn */
    CPSRWriteRaw = 2,             /* trust values, do not switch reg banks */
    CPSRWriteByGDBStub = 3,       /* from the GDB stub */
} CPSRWriteType;

typedef struct CPUARMState {
    uint32_t regs[16];
    int eabi;
    uint64_t features;

    /* System control coprocessor (cp15) */
    struct {
        uint32_t c0_cpuid;
        union { /* Cache size selection */
            struct {
                uint64_t _unused_csselr0;
                uint64_t csselr_ns;
                uint64_t _unused_csselr1;
                uint64_t csselr_s;
            };
            uint64_t csselr_el[4];
        };
        union { /* System control register. */
            struct {
                uint64_t _unused_sctlr;
                uint64_t sctlr_ns;
                uint64_t hsctlr;
                uint64_t sctlr_s;
            };
            uint64_t sctlr_el[4];
        };
        uint64_t cpacr_el1; /* Architectural feature access control register */
        uint64_t cptr_el[4];  /* ARMv8 feature trap registers */
        uint32_t c1_xscaleauxcr; /* XScale auxiliary control register.  */
        uint64_t sder; /* Secure debug enable register. */
        uint32_t nsacr; /* Non-secure access control register. */
        union { /* MMU translation table base 0. */
            struct {
                uint64_t _unused_ttbr0_0;
                uint64_t ttbr0_ns;
                uint64_t _unused_ttbr0_1;
                uint64_t ttbr0_s;
            };
            uint64_t ttbr0_el[4];
        };
        union { /* MMU translation table base 1. */
            struct {
                uint64_t _unused_ttbr1_0;
                uint64_t ttbr1_ns;
                uint64_t _unused_ttbr1_1;
                uint64_t ttbr1_s;
            };
            uint64_t ttbr1_el[4];
        };
        uint64_t vttbr_el2; /* Virtualization Translation Table Base.  */
        /* MMU translation table base control. */
        TCR tcr_el[4];
        TCR vtcr_el2; /* Virtualization Translation Control.  */
        uint32_t c2_data; /* MPU data cacheable bits.  */
        uint32_t c2_insn; /* MPU instruction cacheable bits.  */
        union { /* MMU domain access control register
                 * MPU write buffer control.
                 */
            struct {
                uint64_t dacr_ns;
                uint64_t dacr_s;
            };
            struct {
                uint64_t dacr32_el2;
            };
        };
        uint32_t pmsav5_data_ap; /* PMSAv5 MPU data access permissions */
        uint32_t pmsav5_insn_ap; /* PMSAv5 MPU insn access permissions */
        uint64_t hcr_el2; /* Hypervisor configuration register */
        uint64_t scr_el3; /* Secure configuration register.  */
        union { /* Fault status registers.  */
            struct {
                uint64_t ifsr_ns;
                uint64_t ifsr_s;
            };
            struct {
                uint64_t ifsr32_el2;
            };
        };
        union {
            struct {
                uint64_t _unused_dfsr;
                uint64_t dfsr_ns;
                uint64_t hsr;
                uint64_t dfsr_s;
            };
            uint64_t esr_el[4];
        };
        uint32_t c6_region[8]; /* MPU base/size registers.  */
        union { /* Fault address registers. */
            struct {
                uint64_t _unused_far0;
#ifdef HOST_WORDS_BIGENDIAN
                uint32_t ifar_ns;
                uint32_t dfar_ns;
                uint32_t ifar_s;
                uint32_t dfar_s;
#else
                uint32_t dfar_ns;
                uint32_t ifar_ns;
                uint32_t dfar_s;
                uint32_t ifar_s;
#endif
                uint64_t _unused_far3;
            };
            uint64_t far_el[4];
        };
        uint64_t hpfar_el2;
        uint64_t hstr_el2;
        union { /* Translation result. */
            struct {
                uint64_t _unused_par_0;
                uint64_t par_ns;
                uint64_t _unused_par_1;
                uint64_t par_s;
            };
            uint64_t par_el[4];
        };

        uint32_t c9_insn; /* Cache lockdown registers.  */
        uint32_t c9_data;
        uint64_t c9_pmcr; /* performance monitor control register */
        uint64_t c9_pmcnten; /* perf monitor counter enables */
        uint64_t c9_pmovsr; /* perf monitor overflow status */
        uint64_t c9_pmuserenr; /* perf monitor user enable */
        uint64_t c9_pmselr; /* perf monitor counter selection register */
        uint64_t c9_pminten; /* perf monitor interrupt enables */
        union { /* Memory attribute redirection */
            struct {
#ifdef HOST_WORDS_BIGENDIAN
                uint64_t _unused_mair_0;
                uint32_t mair1_ns;
                uint32_t mair0_ns;
                uint64_t _unused_mair_1;
                uint32_t mair1_s;
                uint32_t mair0_s;
#else
                uint64_t _unused_mair_0;
                uint32_t mair0_ns;
                uint32_t mair1_ns;
                uint64_t _unused_mair_1;
                uint32_t mair0_s;
                uint32_t mair1_s;
#endif
            };
            uint64_t mair_el[4];
        };
        union { /* vector base address register */
            struct {
                uint64_t _unused_vbar;
                uint64_t vbar_ns;
                uint64_t hvbar;
                uint64_t vbar_s;
            };
            uint64_t vbar_el[4];
        };
        uint32_t mvbar; /* (monitor) vector base address register */
        struct { /* FCSE PID. */
            uint32_t fcseidr_ns;
            uint32_t fcseidr_s;
        };
        union { /* Context ID. */
            struct {
                uint64_t _unused_contextidr_0;
                uint64_t contextidr_ns;
                uint64_t _unused_contextidr_1;
                uint64_t contextidr_s;
            };
            uint64_t contextidr_el[4];
        };
        union { /* User RW Thread register. */
            struct {
                uint64_t tpidrurw_ns;
                uint64_t tpidrprw_ns;
                uint64_t htpidr;
                uint64_t _tpidr_el3;
            };
            uint64_t tpidr_el[4];
        };
        /* The secure banks of these registers don't map anywhere */
        uint64_t tpidrurw_s;
        uint64_t tpidrprw_s;
        uint64_t tpidruro_s;

        union { /* User RO Thread register. */
            uint64_t tpidruro_ns;
            uint64_t tpidrro_el[1];
        };
        uint64_t c14_cntfrq; /* Counter Frequency register */
        uint64_t c14_cntkctl; /* Timer Control register */
        uint32_t cnthctl_el2; /* Counter/Timer Hyp Control register */
        uint64_t cntvoff_el2; /* Counter Virtual Offset register */
        ARMGenericTimer c14_timer[NUM_GTIMERS];
        uint32_t c15_cpar; /* XScale Coprocessor Access Register */
        uint32_t c15_ticonfig; /* TI925T configuration byte.  */
        uint32_t c15_i_max; /* Maximum D-cache dirty line index.  */
        uint32_t c15_i_min; /* Minimum D-cache dirty line index.  */
        uint32_t c15_threadid; /* TI debugger thread-ID.  */
        uint32_t c15_config_base_address; /* SCU base address.  */
        uint32_t c15_diagnostic; /* diagnostic register */
        uint32_t c15_power_diagnostic;
        uint32_t c15_power_control; /* power control */
        uint64_t dbgbvr[16]; /* breakpoint value registers */
        uint64_t dbgbcr[16]; /* breakpoint control registers */
        uint64_t dbgwvr[16]; /* watchpoint value registers */
        uint64_t dbgwcr[16]; /* watchpoint control registers */
        uint64_t mdscr_el1;
        uint64_t oslsr_el1; /* OS Lock Status */
        uint64_t mdcr_el2;
        uint64_t mdcr_el3;
        /* Stores the architectural value of the counter *the last time it was
         * updated* by pmccntr_op_start. Accesses should always be surrounded
         * by pmccntr_op_start/pmccntr_op_finish to guarantee the latest
         * architecturally-correct value is being read/set.
         */
        uint64_t c15_ccnt;
        /* Stores the delta between the architectural value and the underlying
         * cycle count during normal operation. It is used to update c15_ccnt
         * to be the correct architectural value before accesses. During
         * accesses, c15_ccnt_delta contains the underlying count being used
         * for the access, after which it reverts to the delta value in
         * pmccntr_op_finish.
         */
        uint64_t c15_ccnt_delta;
        uint64_t c14_pmevcntr[31];
        uint64_t c14_pmevcntr_delta[31];
        uint64_t c14_pmevtyper[31];
        uint64_t pmccfiltr_el0; /* Performance Monitor Filter Register */
        uint64_t vpidr_el2; /* Virtualization Processor ID Register */
        uint64_t vmpidr_el2; /* Virtualization Multiprocessor ID Register */
        uint64_t tfsr_el[4]; /* tfsre0_el1 is index 0.  */
        uint64_t gcr_el1;
        uint64_t rgsr_el1;
    } cp15;
} CPUARMState;
typedef CPUARMState CPUArchState;

typedef struct ARMCPU {
    CPUARMState env;
} ARMCPU;

enum arm_features {
    ARM_FEATURE_AUXCR,  /* ARM1026 Auxiliary control register.  */
    ARM_FEATURE_XSCALE, /* Intel XScale extensions.  */
    ARM_FEATURE_IWMMXT, /* Intel iwMMXt extension.  */
    ARM_FEATURE_V6,
    ARM_FEATURE_V6K,
    ARM_FEATURE_V7,
    ARM_FEATURE_THUMB2,
    ARM_FEATURE_PMSA,   /* no MMU; may have Memory Protection Unit */
    ARM_FEATURE_NEON,
    ARM_FEATURE_M, /* Microcontroller profile.  */
    ARM_FEATURE_OMAPCP, /* OMAP specific CP15 ops handling.  */
    ARM_FEATURE_THUMB2EE,
    ARM_FEATURE_V7MP,    /* v7 Multiprocessing Extensions */
    ARM_FEATURE_V7VE, /* v7 Virtualization Extensions (non-EL2 parts) */
    ARM_FEATURE_V4T,
    ARM_FEATURE_V5,
    ARM_FEATURE_STRONGARM,
    ARM_FEATURE_VAPA, /* cp15 VA to PA lookups */
    ARM_FEATURE_GENERIC_TIMER,
    ARM_FEATURE_MVFR, /* Media and VFP Feature Registers 0 and 1 */
    ARM_FEATURE_DUMMY_C15_REGS, /* RAZ/WI all of cp15 crn=15 */
    ARM_FEATURE_CACHE_TEST_CLEAN, /* 926/1026 style test-and-clean ops */
    ARM_FEATURE_CACHE_DIRTY_REG, /* 1136/1176 cache dirty status register */
    ARM_FEATURE_CACHE_BLOCK_OPS, /* v6 optional cache block operations */
    ARM_FEATURE_MPIDR, /* has cp15 MPIDR */
    ARM_FEATURE_PXN, /* has Privileged Execute Never bit */
    ARM_FEATURE_LPAE, /* has Large Physical Address Extension */
    ARM_FEATURE_V8,
    ARM_FEATURE_AARCH64, /* supports 64 bit mode */
    ARM_FEATURE_CBAR, /* has cp15 CBAR */
    ARM_FEATURE_CBAR_RO, /* has cp15 CBAR and it is read-only */
    ARM_FEATURE_EL2, /* has EL2 Virtualization support */
    ARM_FEATURE_EL3, /* has EL3 Secure monitor support */
    ARM_FEATURE_THUMB_DSP, /* DSP insns supported in the Thumb encodings */
    ARM_FEATURE_PMU, /* has PMU support */
    ARM_FEATURE_VBAR, /* has cp15 VBAR */
    ARM_FEATURE_M_SECURITY, /* M profile Security Extension */
    ARM_FEATURE_M_MAIN, /* M profile Main Extension */
};

static inline int arm_feature(CPUARMState *env, int feature)
{
    return (env->features & (1ULL << feature)) != 0;
}

static inline bool arm_is_secure_below_el3(CPUARMState *env)
{
    if (arm_feature(env, ARM_FEATURE_EL3)) {
        return !(env->cp15.scr_el3 & SCR_NS);
    } else {
        /* If EL3 is not supported then the secure state is implementation
         * defined, in which case QEMU defaults to non-secure.
         */
        return false;
    }
}

static inline bool arm_el_is_aa64(CPUARMState *env, int el)
{
    /* This isn't valid for EL0 (if we're in EL0, is_a64() is what you want,
     * and if we're not in EL0 then the state of EL0 isn't well defined.)
     */
    assert(el >= 1 && el <= 3);
    bool aa64 = arm_feature(env, ARM_FEATURE_AARCH64);

    /* The highest exception level is always at the maximum supported
     * register width, and then lower levels have a register width controlled
     * by bits in the SCR or HCR registers.
     */
    if (el == 3) {
        return aa64;
    }

    if (arm_feature(env, ARM_FEATURE_EL3)) {
        aa64 = aa64 && (env->cp15.scr_el3 & SCR_RW);
    }

    if (el == 2) {
        return aa64;
    }

    if (arm_feature(env, ARM_FEATURE_EL2) && !arm_is_secure_below_el3(env)) {
        aa64 = aa64 && (env->cp15.hcr_el2 & HCR_RW);
    }

    return aa64;
}

static inline bool access_secure_reg(CPUARMState *env)
{
    bool ret = (arm_feature(env, ARM_FEATURE_EL3) &&
                !arm_el_is_aa64(env, 3) &&
                !(env->cp15.scr_el3 & SCR_NS));

    return ret;
}


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
