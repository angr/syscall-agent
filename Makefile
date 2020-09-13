CFLAGS=-std=gnu17 -D_GNU_SOURCE -Iinclude -DTARGET_I386 -DTARGET_X86_64 -Iqemu/include -Iqemu/include/qemu -Iqemu/linux-user -Iqemu/linux-user/host/x86_64 -Iqemu/linux-user/x86_64 -Iqemu/target/i386 $(shell pkg-config --cflags glib-2.0)
# add -DHOST_WORDS_BIGENDIAN and -DTARGET_WORDS_BIGENDIAN as appropriate
LFLAGS=$(shell pkg-config --libs glib-2.0) -lpthread

OBJECTS=qemu/linux-user/syscall.c.o qemu/linux-user/uname.c.o qemu/linux-user/fd-trans.c.o qemu/linux-user/safe-syscall.S.o qemu/linux-user/uaccess.c.o qemu/thunk.c.o qemu/linux-user/signal.c.o main.c.o

main: $(OBJECTS)
	$(CC) $? $(LFLAGS) -o $@

clean:
	rm -f $(OBJECTS) main

%.S.o: %.S
	$(CC) -c $? $(CFLAGS) -o $@

%.c.o: %.c
	$(CC) -c $? $(CFLAGS) -o $@
