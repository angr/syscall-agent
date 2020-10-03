#include <stdint.h>
#include <string.h>
#include <setjmp.h>

#include "qemu/osdep.h"
#include "qemu.h"
#include "panic.h"
#include "exec/cpu_ldst.h"

//
// Globals
//

char *exec_path;
const char *qemu_uname_release;
__thread CPUState *thread_cpu;
CPUState cpu;
CPUArchState cpu_arch;
TaskState task;
target_ulong *task_auxv;
int qemu_loglevel;

jmp_buf jmpout;
int cur_sysno;

//
// Memory record API
//

typedef struct memrecord {
    char *data;
    target_ulong addr;
    size_t size;
    // Maaayyyyybe this turns into a list of ranges for which we're writing in the future
    bool writing;
    struct memrecord *next;
} memrecord;

memrecord *memory;

memrecord *memory_new(void *data, target_ulong addr, size_t size, bool writing) {
        memrecord *newrec = malloc(sizeof(memrecord));
        newrec->addr = addr;
        newrec->size = size;
        newrec->writing = writing;
        newrec->data = data;
        return newrec;
}

void memory_insert(memrecord **loc, memrecord *data) {
    data->next = *loc;
    *loc = data;
}

// returns a pointer to the memrecord pointer for the first entry containing bytes at or after addr, or a pointer to the final null pointer if the address comes after all the resident records
memrecord **memory_find(target_ulong addr) {
    memrecord **result = &memory;
    while (*result != NULL) {
        if ((*result)->addr + (*result)->size >= addr) {
            break;
        }
        result = &(*result)->next;
    }
    return result;
}

void *memory_translate(target_ulong addr) {
    memrecord *cur = *memory_find(addr);
    if (cur == NULL || !(cur->addr <= addr && addr < cur->addr + cur->size)) {
        return NULL;
    }
    return cur->data + (addr - cur->addr);
}

void memory_coalesce() {
    memrecord *cur = memory;
    while (cur != NULL && cur->next != NULL) {
        // TODO sanity check that there are no overlaps
        if (cur->addr + cur->size == cur->next->addr) {
            char *newdata = malloc(cur->size + cur->next->size);
            memcpy(newdata, cur->data, cur->size);
            memcpy(newdata + cur->size, cur->next->data, cur->next->size);

            free(cur->data);
            free(cur->next->data);
            cur->data = newdata;
            cur->writing |= cur->next->writing;

            memrecord *orphan = cur->next;
            cur->next = orphan->next;
            free(orphan);
        }
        cur = cur->next;
    }
}

void memory_reset() {
    memrecord *cur = memory;
    while (cur != NULL) {
        memrecord *next = cur->next;
        free(cur->data);
        free(cur);
        cur = next;
    }

    memory = NULL;
}

//
// IPC outbound
//

target_ulong _target_strlen(target_ulong addr) {
    // ipc
}

void *retrieve_memory(target_ulong addr, target_ulong size, bool writing) {
    void *result = NULL;
    //ipc

    if (result == NULL) {
        longjmp(jmpout, 1);
    }
    return result;
}

void sync_one_memory(target_ulong addr, void *data, size_t size) {
    // ipc
}

void sync_memory() {
    memrecord *cur = memory;
    while (cur != NULL) {
        if (cur->writing) {
            sync_one_memory(cur->addr, cur->data, cur->size);
        }
        cur = cur->next;
    }

    memory_reset();
}

//
// IPC inbound (entry points)
//

target_ulong syscall_main(int sysno, target_ulong *args, int argc) {
    abi_long result = -1;
    cur_sysno = sysno;
    if (setjmp(jmpout) == 0) {
#define sysarg(n) (n < argc ? args[n] : 0)
        result = do_syscall(&cpu_arch, sysno, sysarg(0), sysarg(1), sysarg(2), sysarg(3), sysarg(4), sysarg(5), sysarg(6), sysarg(7));

        sync_memory();
    } else {
        memory_reset();
    }

    return result;
}

void set_auxv(target_ulong *auxv, int auxv_count) {
    free(task_auxv);
    task_auxv = malloc(sizeof(target_ulong) * 2 * (auxv_count + 1));
    for (int i = 0; i < auxv_count; i++) {
        task_auxv[2*i] = auxv[2*i];
        task_auxv[2*i+1] = auxv[2*i+1];
    }
    task_auxv[auxv_count] = 0;
    task_auxv[auxv_count+1] = 0;
}

//
// Startup
//

void init() {
    thread_cpu = &cpu;
    cpu.opaque = &task;
}

int main(int argc, char **argv) {
    init();
    // ipc loop
    return 0;
}

//
// QEMU APIs
//

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
void *g2h(target_ulong addr) {
    bool retrying = false;
    if (0) {
        retry:
        retrying = true;
    }
    void *result = memory_translate(addr);
    if (result == NULL) {
        if (!retrying) {
            switch (cur_sysno) {
#ifdef TARGET_NR_futex
                case TARGET_NR_futex:
#endif
#ifdef TARGET_NR_futex_time64
                case TARGET_NR_futex_time64:
#endif
                    page_check_range(addr, 4, 1);
                    goto retry;
                case TARGET_NR_mount:
                    page_check_range(addr, target_strlen(addr), 0);
                    goto retry;
            }
        }
        fprintf(stderr, "Unlocked access to guest memory\n");
        abort();
    }
    return result;
}

int page_check_range(target_ulong addr, target_ulong size_, int flags) {
    ssize_t size = size_;
    memrecord **cur = memory_find(addr);
    while (size > 0) {
        ssize_t subsize;
        if (*cur == NULL) {
            // fell off the end
            memory_insert(cur, memory_new(retrieve_memory(addr, size, flags), addr, size, flags));
            subsize = size;
        } else if ((*cur)->addr > addr) {
            // need to retrieve data from the start of the range
            subsize = (*cur)->addr - addr;
            if (subsize > size) {
                subsize = size;
            }

            memory_insert(cur, memory_new(retrieve_memory(addr, subsize, flags), addr, subsize, flags));
        } else {
            // data is present. scan forward.
            (*cur)->writing |= flags;
            subsize = (*cur)->size;
        }
        addr += subsize;
        size -= subsize;
        cur = &(*cur)->next;
    }

    memory_coalesce();

    return 1;
}

unsigned long qemu_getauxval(unsigned long key) {
    if (task_auxv == 0) {
        return 0;
    }

    for (int i = 0; ; i++) {
        if (task_auxv[i*2] == 0) {
            return 0;
        }
        if (task_auxv[i*2] == key) {
            return task_auxv[i*2+1];
        }
    }
}

// these are copied directly from uaccess.c because we need to override target_strlen
abi_long copy_from_user(void *hptr, abi_ulong gaddr, size_t len)
{
    abi_long ret = 0;
    void *ghptr;

    if ((ghptr = lock_user(VERIFY_READ, gaddr, len, 1))) {
        memcpy(hptr, ghptr, len);
        unlock_user(ghptr, gaddr, 0);
    } else
        ret = -TARGET_EFAULT;

    return ret;
}


abi_long copy_to_user(abi_ulong gaddr, void *hptr, size_t len)
{
    abi_long ret = 0;
    void *ghptr;

    if ((ghptr = lock_user(VERIFY_WRITE, gaddr, len, 0))) {
        memcpy(ghptr, hptr, len);
        unlock_user(ghptr, gaddr, len);
    } else
        ret = -TARGET_EFAULT;

    return ret;
}

/* Return the length of a string in target memory or -TARGET_EFAULT if
   access error  */
abi_long target_strlen(abi_ulong guest_addr1)
{
    return _target_strlen(guest_addr1);
}

//
// Unsupported QEMU APIs
//

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
