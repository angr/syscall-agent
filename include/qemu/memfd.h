#include <sys/syscall.h>

// hack: use the presence of the memfd syscall as a proxy for the presence of the glibc wrapper
#ifndef __NR_memfd_create
inline int memfd_create(const char *name, unsigned int flags) {
	errno = ENOSYS;
	return -1;
}
#endif
