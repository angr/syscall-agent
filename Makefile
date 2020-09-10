QEMU_CFLAGS=-std=gnu17 -D_GNU_SOURCE -DTARGET_I386 -DTARGET_X86_64 -Iinclude -Iqemu/include -Iqemu/include/qemu -Iqemu/linux-user/host/x86_64 -Iqemu/linux-user/x86_64 -Iqemu/target/i386 $(shell pkg-config --cflags glib-2.0)
# add -DHOST_WORDS_BIGENDIAN and -DTARGET_WORDS_BIGENDIAN as appropriate

qemu/%.o: qemu/%.c
	$(CC) -c $? $(QEMU_CFLAGS) -o $@
