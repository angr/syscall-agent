#include <stdint.h>
#include <string.h>

#include "qemu/osdep.h"
#include "qemu.h"
#include "panic.h"
#include "exec/cpu_ldst.h"

char *exec_path;
const char *qemu_uname_release;
__thread CPUState *thread_cpu;
int qemu_loglevel;

int qemu_strnlen(const char *s, int max_len) { return strnlen(s, max_len); }
CPUState *env_cpu(CPUArchState *env) { return thread_cpu; }
int qemu_log(const char *fmt, ...) { va_list ap; va_start(ap, fmt); vfprintf(stderr, fmt, ap); }

/* cases where g2h is called unlocked
 *
 * brk - unsupported
 * shmat/shmdt - unsupported
 * write ldt, get/set_thread_area - unsupported
 * futex - always a 32-bit word
 * reading /proc/self/maps - unsupported
 * exit - unsupported
 * mount: this is a fixme in qemu because the argument means different things to different drivers. Generally it is a null-terminated string though.
 * msync/mlock/munlock - highly questionable. maybe call this unsupported?
 * set_tid_address - unsupported I guess???
 *
 */
void *g2h(target_ulong addr) { PANIC_UNIMPLEMENTED(); }
int page_check_range(target_ulong addr, target_ulong size, int flags) { PANIC_UNIMPLEMENTED(); }
unsigned long qemu_getauxval(unsigned long key) { PANIC_UNIMPLEMENTED(); }

int main(int argc, char **argv) {
    return 0;
}


// reading /proc/self/maps is unsupported. come on
GSList *read_self_maps(void) { PANIC_UNREACHED(); }
void free_self_maps(GSList *info) { PANIC_UNREACHED(); }
int page_get_flags(target_ulong addr) { PANIC_UNREACHED(); }
int page_set_flags(target_ulong addr, target_ulong size, int flags) { PANIC_UNREACHED(); }

// mmap and friends are unsupported
abi_long target_mmap(abi_ulong start, abi_ulong len, int prot,
                     int flags, int fd, abi_ulong offset) { PANIC_UNREACHED(); }
int target_mprotect(abi_ulong start, abi_ulong len, int prot) { PANIC_UNREACHED(); }
abi_long target_mremap(abi_ulong old_addr, abi_ulong old_size,
                       abi_ulong new_size, unsigned long flags,
                       abi_ulong new_addr) { PANIC_UNREACHED(); }
int target_munmap(abi_ulong start, abi_ulong len) { PANIC_UNREACHED(); }
abi_ulong mmap_find_vma(abi_ulong a, abi_ulong b, abi_ulong c) { PANIC_UNREACHED(); }
void mmap_lock(void) { PANIC_UNREACHED(); }
void mmap_unlock(void) { PANIC_UNREACHED(); }
bool have_mmap_lock(void) { PANIC_UNREACHED(); }

// clone is unsupported
void init_task_state(TaskState *ts) { PANIC_UNREACHED(); }
void fork_start(void) { PANIC_UNREACHED(); }
void fork_end(int x) { PANIC_UNREACHED(); }

// sigreturn is unsupported
long do_rt_sigreturn(CPUArchState *env) { PANIC_UNREACHED(); }

// exit is unsupported
void cpu_exit(CPUState *cpu) { PANIC_UNREACHED(); }
void preexit_cleanup(CPUArchState *env, int code) { PANIC_UNREACHED(); }

// h2g is only used for mmap, signals, reading /proc/self/maps, and shmat
target_ulong h2g(unsigned long ptr) { PANIC_UNREACHED(); }
bool h2g_valid(unsigned long ptr) { PANIC_UNREACHED(); }
