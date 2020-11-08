CFLAGS+=-std=gnu99 -D_GNU_SOURCE -DCONFIG_LINUX -DCONFIG_USER_ONLY -Iinclude -Iqemu/include -Iqemu/include/qemu -Iqemu/linux-user $(shell pkg-config --cflags glib-2.0)
ifdef DEBUG
CFLAGS+=-g -O0
endif

LFLAGS+=$(shell pkg-config --libs glib-2.0) -lpthread -lrt -lzmq

EXECUTABLE="syscall-agent-$(TARGET)"

AUTO_ENDNESS=$(shell echo I | tr -d [:space:] | od -to2 | head -n1 | awk '{print $$2}' | cut -c6)
AUTO_ARCH=$(shell uname -m)
AUTO_HOST=$(AUTO_ARCH)_$(AUTO_ENDNESS)

ifeq "$(HOST)" ""
ifeq "$(AUTO_HOST)" "x86_64_1"
HOST=x86_64
else ifeq "$(AUTO_HOST)" "mips_0"
HOST=mips
else ifeq "$(AUTO_HOST)" "mips_1"
HOST=mipsel
else ifeq "$(AUTO_HOST)" "armv7l_0"
HOST=armeb
else ifeq "$(AUTO_HOST)" "armv6l_0"
HOST=armeb
else ifeq "$(AUTO_HOST)" "armv7l_1"
HOST=arm
else ifeq "$(AUTO_HOST)" "armv6l_1"
HOST=arm
endif
endif

ifeq "$(TARGET)" ""
$(info Defaulting to TARGET=HOST. To override, specify TARGET variable.)
TARGET=$(HOST)
endif

# host config
ifeq "$(HOST)" "x86_64"
CFLAGS+=-Iqemu/linux-user/host/x86_64
else ifeq "$(HOST)" "mips"
CFLAGS+=-Iqemu/linux-user/host/mips -DHOST_WORDS_BIGENDIAN
else ifeq "$(HOST)" "mipsel"
CFLAGS+=-Iqemu/linux-user/host/mips
else ifeq "$(HOST)" "arm"
CFLAGS+=-Iqemu/linux-user/host/arm
else ifeq "$(HOST)" "armeb"
CFLAGS+=-Iqemu/linux-user/host/arm -DHOST_WORDS_BIGENDIAN
else ifneq "$(MAKECMDGOALS)" "clean"
$(error Unsupported HOST. See makefile for list of supported targets.)
endif

# target config
ifeq "$(TARGET)" "aarch64_be"
CFLAGS+=-DTARGET_AARCH64 -DTARGET_WORDS_BIGENDIAN -Iqemu/target/arm -Iqemu/linux-user/aarch64
else ifeq "$(TARGET)" "aarch64"
CFLAGS+=-DTARGET_AARCH64 -Iqemu/target/arm -Iqemu/linux-user/aarch64
else ifeq "$(TARGET)" "armeb"
CFLAGS+=-DTARGET_ARM -DTARGET_WORDS_BIGENDIAN -Iqemu/target/arm -Iqemu/linux-user/arm
else ifeq "$(TARGET)" "arm"
CFLAGS+=-DTARGET_ARM -Iqemu/target/arm -Iqemu/linux-user/arm
else ifeq "$(TARGET)" "i386"
CFLAGS+=-DTARGET_I386 -Iqemu/target/i386 -Iqemu/linux-user/i386
else ifeq "$(TARGET)" "mips64el"
CFLAGS+=-DTARGET_MIPS64 -DTARGET_MIPS -DTARGET_ABI_MIPSN64 -Iqemu/target/mips -Iqemu/linux-user/mips64
else ifeq "$(TARGET)" "mips64"
CFLAGS+=-DTARGET_MIPS64 -DTARGET_MIPS -DTARGET_ABI_MIPSN64 -DTARGET_WORDS_BIGENDIAN -Iqemu/target/mips -Iqemu/linux-user/mips64
else ifeq "$(TARGET)" "mipsel"
CFLAGS+=-DTARGET_MIPS -DTARGET_ABI_MIPSO32 -Iqemu/target/mips -Iqemu/linux-user/mips
else ifeq "$(TARGET)" "mipsn32el"
CFLAGS+=-DTARGET_MIPS64 -DTARGET_MIPS -DTARGET_ABI_MIPSN32 -Iqemu/target/mips -Iqemu/linux-user/mips64
else ifeq "$(TARGET)" "mipsn32"
CFLAGS+=-DTARGET_MIPS64 -DTARGET_MIPS -DTARGET_ABI_MIPSN32 -DTARGET_WORDS_BIGENDIAN -Iqemu/target/mips -Iqemu/linux-user/mips64
else ifeq "$(TARGET)" "mips"
CFLAGS+=-DTARGET_MIPS -DTARGET_ABI_MIPSO32 -DTARGET_WORDS_BIGENDIAN -Iqemu/target/mips -Iqemu/linux-user/mips
else ifeq "$(TARGET)" "ppc64abi32"
CFLAGS+=-DTARGET_PPC64 -DTARGET_ABI32 -DTARGET_PPC -DTARGET_WORDS_BIGENDIAN -Iqemu/target/ppc -Iqemu/linux-user/ppc
else ifeq "$(TARGET)" "ppc64le"
CFLAGS+=-DTARGET_PPC64 -DTARGET_PPC -Iqemu/target/ppc -Iqemu/linux-user/ppc
else ifeq "$(TARGET)" "ppc64"
CFLAGS+=-DTARGET_PPC64 -DTARGET_PPC -DTARGET_WORDS_BIGENDIAN -Iqemu/target/ppc -Iqemu/linux-user/ppc
else ifeq "$(TARGET)" "ppc"
CFLAGS+=-DTARGET_PPC -DTARGET_WORDS_BIGENDIAN -Iqemu/target/ppc -Iqemu/linux-user/ppc
else ifeq "$(TARGET)" "x86_64"
CFLAGS+=-DTARGET_X86_64 -DTARGET_I386 -Iqemu/target/i386 -Iqemu/linux-user/x86_64
else ifneq "$(MAKECMDGOALS)" "clean"
$(error Unsupported TARGET. See makefile for list of supported targets.)
endif

OBJECTS=build/$(TARGET)/qemu/linux-user/syscall.c.o build/$(TARGET)/qemu/linux-user/uname.c.o build/$(TARGET)/qemu/linux-user/fd-trans.c.o build/$(TARGET)/qemu/linux-user/safe-syscall.S.o build/$(TARGET)/qemu/thunk.c.o build/$(TARGET)/qemu/linux-user/signal.c.o build/$(TARGET)/qemu/linux-user/strace.c.o build/$(TARGET)/main.c.o

$(EXECUTABLE): $(OBJECTS)
	$(CC) $^ $(LFLAGS) -o $@

clean:
	rm -rf build syscall-agent-*

build/$(TARGET)/%.S.o: %.S
	mkdir -p $(shell dirname $@)
	$(CC) -c $^ $(CFLAGS) -o $@

build/$(TARGET)/%.c.o: %.c
	mkdir -p $(shell dirname $@)
	$(CC) -c $^ $(CFLAGS) -o $@
