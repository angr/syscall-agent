#!/bin/bash -e

if [[ -z "$1" ]]; then
	echo "Usage: $0 [path_to_qemu]"
	exit 1
fi

pushd $1
rm -rf build
./configure --target-list=arm-linux-user,i386-linux-user,x86_64-linux-user,aarch64-linux-user,mips-linux-user,mips64-linux-user,ppc-linux-user
cd build
./ninjatool -t ninja2make --omit clean dist uninstall cscope TAGS ctags < build.ninja > Makefile.ninja
for sup in $(grep syscall.*\.h: Makefile.ninja | cut -d: -f1); do
	make -f Makefile.ninja $sup
	arch=$(cut -d- -f2 <<<$sup)
	cp $sup ../linux-user/$arch/
done
popd

rm -rf qemu
mkdir qemu
for subdir in linux-user util include include/qemu include/fpu include/exec include/exec/user linux-user/host linux-user/host/x86_64 target target/x86_64; do
	mkdir qemu/$subdir
done

for fname in include/qemu/compiler.h linux-user/errno_defs.h linux-user/fd-trans.c linux-user/fd-trans.h include/glib-compat.h linux-user/ioctls.h linux-user/linux_loop.h linux-user/qemu.h include/qemu/selfmap.h linux-user/socket.h linux-user/syscall.c linux-user/syscall_defs.h linux-user/syscall_types.h linux-user/uname.c linux-user/uname.h include/exec/user/abitypes.h include/exec/user/thunk.h linux-user/host/x86_64/hostdep.h linux-user/host/x86_64/safe-syscall.inc.S linux-user/safe-syscall.S thunk.c include/qemu/cutils.h include/fpu/softfloat-types.h include/qemu/bswap.h include/qemu/host-utils.h include/qemu/bitops.h include/qemu/atomic.h linux-user/signal.c linux-user/signal-common.h linux-user/strace.c linux-user/strace.list; do
	cp $1/$fname qemu/$fname
done

cp -r $1/linux-user/generic qemu/linux-user/generic

for arch in i386 x86_64 aarch64 mips mips64; do #arm ppc missing syscall_nr.h?
	mkdir qemu/linux-user/$arch
	for fname in target_cpu.h sockbits.h target_cpu.h target_fcntl.h target_structs.h target_syscall.h target_signal.h termbits.h; do
		cp $1/linux-user/$arch/$fname qemu/linux-user/$arch
	done
	cp $1/linux-user/$arch/syscall*.h qemu/linux-user/$arch 2>/dev/null || true
	if [ -d "$1/target/$arch" ]; then
		mkdir qemu/target/$arch
		cp $1/target/$arch/cpu-param.h qemu/target/$arch
	fi
done

#mkdir qemu/linux-user/arm/nwfpe
#cp $1/linux-user/arm/nwfpe/fpa11.h qemu/linux-user/arm/nwfpe
