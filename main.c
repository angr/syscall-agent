#include <stdint.h>
#include <string.h>

#include "qemu/osdep.h"
#include "qemu.h"
#include "panic.h"
#include "exec/cpu_ldst.h"

int main(int argc, char **argv) {
    return 0;
}

int qemu_strnlen(const char *s, int max_len) { return strnlen(s, max_len); }

int page_get_flags(target_ulong addr) { PANIC_UNIMPLEMENTED(); }
int page_set_flags(target_ulong addr, target_ulong size, int flags) { PANIC_UNIMPLEMENTED(); }
int page_check_range(target_ulong addr, target_ulong size, int flags) { PANIC_UNIMPLEMENTED(); }

void *g2h(target_ulong addr) { PANIC_UNIMPLEMENTED(); }
target_ulong h2g(unsigned long ptr) { PANIC_UNIMPLEMENTED(); }
bool h2g_valid(unsigned long ptr) { PANIC_UNIMPLEMENTED(); }

CPUState *env_cpu(CPUArchState *env) { PANIC_UNIMPLEMENTED(); }

void mmap_lock(void) { PANIC_UNIMPLEMENTED(); }
void mmap_unlock(void) { PANIC_UNIMPLEMENTED(); }
bool have_mmap_lock(void) { PANIC_UNIMPLEMENTED(); }

int qemu_log(const char *fmt, ...) { PANIC_UNIMPLEMENTED(); }

GSList *read_self_maps(void) { PANIC_UNIMPLEMENTED(); }
void free_self_maps(GSList *info) { PANIC_UNIMPLEMENTED(); }

abi_long target_mmap(abi_ulong start, abi_ulong len, int prot,
                     int flags, int fd, abi_ulong offset) { PANIC_UNREACHED(); }
int target_mprotect(abi_ulong start, abi_ulong len, int prot) { PANIC_UNREACHED(); }
abi_long target_mremap(abi_ulong old_addr, abi_ulong old_size,
                       abi_ulong new_size, unsigned long flags,
                       abi_ulong new_addr) { PANIC_UNREACHED(); }
void init_task_state(TaskState *ts) { PANIC_UNREACHED(); }
int target_munmap(abi_ulong start, abi_ulong len) { PANIC_UNREACHED(); }
abi_ulong mmap_find_vma(abi_ulong a, abi_ulong b, abi_ulong c) { PANIC_UNREACHED(); }
void fork_start(void) { PANIC_UNREACHED(); }
void fork_end(int x) { PANIC_UNREACHED(); }
long do_rt_sigreturn(CPUArchState *env) { PANIC_UNREACHED(); }  // maybe? idk do we want to emulate sigreturn
void cpu_exit(CPUState *cpu) { PANIC_UNREACHED(); }

unsigned long qemu_getauxval(unsigned long key) { PANIC_UNIMPLEMENTED(); }
char *exec_path;
const char *qemu_uname_release;
void preexit_cleanup(CPUArchState *env, int code) { PANIC_UNIMPLEMENTED(); }

__thread CPUState *thread_cpu;
int qemu_loglevel;
