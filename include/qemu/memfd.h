#include "qemu/osdep.h"

#if defined CONFIG_LINUX && !defined CONFIG_MEMFD
inline int memfd_create(const char *name, unsigned int flags) {
	errno = ENOSYS;
	return -1;
}
#endif
