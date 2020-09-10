#pragma once

#include <stdint.h>

#include "cpu.h"

uint8_t ldub_p(uint8_t *ptr);
uint16_t lduw_le_p(uint16_t *ptr);
uint32_t ldl_le_p(uint32_t *ptr);
uint64_t ldq_le_p(uint64_t *ptr);

void stb_p(uint8_t *ptr, uint8_t data);
void stw_le_p(uint16_t *ptr, uint16_t data);
void stl_le_p(uint32_t *ptr, uint32_t data);
void stq_le_p(uint64_t *ptr, uint64_t data);

// some of these are normally defined in cpu-all.h but they are important for
// the intercept footprint to stay inside this file
int page_get_flags(target_ulong addr);
int page_set_flags(target_ulong addr, target_ulong size, int flags);
int page_check_range(target_ulong addr, target_ulong size, int flags);

void *g2h(target_ulong addr);
target_ulong h2g(unsigned long);

// pure copied
#if TARGET_VIRT_ADDR_SPACE_BITS <= 32
typedef uint32_t abi_ptr;
#define TARGET_ABI_FMT_ptr "%x"
#else
typedef uint64_t abi_ptr;
#define TARGET_ABI_FMT_ptr "%"PRIx64
#endif

#if HOST_LONG_BITS <= TARGET_VIRT_ADDR_SPACE_BITS
#define guest_addr_valid(x) (1)
#else
#define guest_addr_valid(x) ((x) <= GUEST_ADDR_MAX)
#endif
bool h2g_valid(unsigned long);

static inline int guest_range_valid(unsigned long start, unsigned long len)
{
    return len - 1 <= GUEST_ADDR_MAX && start <= GUEST_ADDR_MAX - len + 1;
}
