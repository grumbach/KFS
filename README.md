# KAGRUM

Writing a Kernel From Scratch

# Prerequisites

## Install Software

```bash
# gcc dependencies
sudo apt install -y build-essential bison flex libgmp3-dev libmpc-dev libmpfr-dev texinfo
# kvm
sudo apt-get install -y qemu-kvm libvirt-bin qemu
# grub
sudo apt-get install -y grub-pc
# gdb
sudo apt-get install -y gdb
```

## Install cross-compiler for i686-elf

```bash
export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

cd $HOME
mkdir -pv opt/cross/bin src

cd $HOME/src
wget http://ftp.gnu.org/gnu/binutils/binutils-2.32.tar.xz
wget http://ftp.gnu.org/gnu/gcc/gcc-8.2.0/gcc-8.2.0.tar.xz
wget http://www.nasm.us/pub/nasm/releasebuilds/2.14.02/nasm-2.14.02.tar.xz

# —- install Binutils —-
tar xf binutils-2.32.tar.xz
cd binutils-2.32/
mkdir build-binutils
cd build-binutils
../configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
sudo make install
cd ../..
rm -rf binutils-2.32

# —- install gcc —-
tar xf gcc-8.2.0.tar.xz
cd gcc-8.2.0/
# The $PREFIX/bin dir _must_ be in the PATH. We did that above.
which -- $TARGET-as || echo $TARGET-as is not in the PATH
mkdir build-gcc
cd build-gcc
../configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc
make all-target-libgcc
sudo make install-gcc
sudo make install-target-libgcc
cd ../..
rm -rf gcc-8.2.0

# —- install nasm —-
tar xf nasm-2.14.02.tar.xz
cd nasm-2.14.02
./configure --prefix="$PREFIX" --target=$TARGET
make all
sudo make install
cd ..
rm -rf nasm-2.14.02

# —- add to PATH —-
export PATH="$HOME/opt/cross/bin:$PATH"
```

## Install KVM

```bash
sudo modprobe kvm-intel
```

# Building the Kernel

```
boot.s
kernel.c
linker.ld
```

## boot.s

```bash
nasm -f elf -o boot.o boot.s
# or
i686-elf-as boot.s -o boot.o
```

## kernel.c

```bash
i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
```

## Linking

```bash
i686-elf-gcc -T linker.ld -o Kagrum -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc
```

# Setup GRUB Bootloader

Check Kagrum with
```bash
grub-file --is-x86-multiboot Kagrum && echo multiboot OK || echo NOT multiboot
```

```bash
# create grub config file
cat << EOF > grub.cfg
menuentry "Kagrum" {
	multiboot /boot/Kagrum
}
EOF

# make a bootable image
mkdir -p isodir/boot/grub
cp Kagrum isodir/boot/Kagrum
cp grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o Kagrum.iso isodir
```

# Virtualisation with QEMU

```bash
# Boot with
qemu-system-i386 -cdrom Kagrum.iso -curses
# or directly with
qemu-system-i386 -kernel Kagrum -curses
```

# Debugging with KVM

```bash
# launch kvm
kvm -m 2048 -s hda Kagrum.iso -redir tcp:2323::23 -curses

# debug kernel in gdb
(gdb) target remote:1234
```

# Booting from a USB Stick

```bash
USB_BLOCK_DEVICE=/dev/sdx
sudo dd if=Kagrum.iso of=$USB_BLOCK_DEVICE && sync
```

# References

* [OSdev Bare Bones](https://wiki.osdev.org/Bare_Bones)
* [GCC cross compiler](https://wiki.osdev.org/GCC_Cross-Compiler)
* [KVM on headless ubuntu](https://www.cyberciti.biz/faq/installing-kvm-on-ubuntu-16-04-lts-server/)
