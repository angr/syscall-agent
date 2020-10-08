CFLAGS=-std=gnu17 -D_GNU_SOURCE -DCONFIG_USER_ONLY -Iinclude -Iqemu/include -Iqemu/include/qemu -Iqemu/linux-user -Iqemu/linux-user/host/x86_64 $(shell pkg-config --cflags glib-2.0) -g -O0
CFLAGS+=-DTARGET_MIPS -DTARGET_ABI_MIPSO32 -Iqemu/target/mips -Iqemu/linux-user/mips
#CFLAGS+=-DTARGET_AMD64 -DTARGET_X86 -Iqemu/target/i386 -Iqemu/linux-user/x86_64
# add -DHOST_WORDS_BIGENDIAN and -DTARGET_WORDS_BIGENDIAN as appropriate
LFLAGS=$(shell pkg-config --libs glib-2.0) -lpthread -lrt -lzmq

OBJECTS=qemu/linux-user/syscall.c.o qemu/linux-user/uname.c.o qemu/linux-user/fd-trans.c.o qemu/linux-user/safe-syscall.S.o qemu/thunk.c.o qemu/linux-user/signal.c.o qemu/linux-user/strace.c.o main.c.o

main: $(OBJECTS)
	$(CC) $^ $(LFLAGS) -o $@

syscall_defs.txt:
	./extract_syscall_defs.py > $@

clean:
	rm -f $(OBJECTS) main

%.S.o: %.S
	$(CC) -c $^ $(CFLAGS) -o $@

%.c.o: %.c
	$(CC) -c $^ $(CFLAGS) -o $@
