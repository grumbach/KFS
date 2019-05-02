# KFS

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

# —- install Binutils —-
cd $HOME/src
 
mkdir build-binutils
cd build-binutils
../binutils-x.y.z/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
make install

# —- install gcc —-
cd $HOME/src
 
# The $PREFIX/bin dir _must_ be in the PATH. We did that above.
which -- $TARGET-as || echo $TARGET-as is not in the PATH
 
mkdir build-gcc
cd build-gcc
../gcc-x.y.z/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc

# —- install nasm —-
./configure --prefix=$PREFIX --target=$TARGET
make all
make install

# —- add to PATH —-
export PATH="$HOME/opt/cross/bin:$PATH"
```

# Building the Kernel

```
boot.s
kernel.c
linker.ld
```

## boot.s

```asm
; multiboot constants
.set ALIGN,    1<<0             /* align loaded modules on page boundaries */
.set MEMINFO,  1<<1             /* provide memory map */
.set FLAGS,    ALIGN | MEMINFO  /* this is the Multiboot 'flag' field */
.set MAGIC,    0x1BADB002       /* 'magic number' lets bootloader find the header */
.set CHECKSUM, -(MAGIC + FLAGS) /* checksum of above, to prove we are multiboot */

; multiboot header 
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

; create some space for stack
.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

; kernel start
.section text
.global _start
.type _start, @function
_start:
    ; setup stack
    mov esp, $stack_top
    ; load GDT, enable paging...
    ; enter high-level kernel
    call kernel_main
    ; cli ; disable interrupts
infinite_loop:
    hlt
    jmp infinite_loop

; set _start size for debug
.size _start, . - _start
```

Assemble with: 
```bash
nasm -f elf -o boot.o boot.s
# or
i686-elf-as boot.s -o boot.o
```

## kernel.c

```c
#include <stdbool.h>  /* for bool datatype */
#include <stddef.h>   /* for size_t and NULL */
#include <stdint.h>   /* fot intx_t and uintx_t */

/* stop fools who didn’t use a cross compiler */
#if defined(__linux__)
# error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* stop fools who aren’t in 32-bit ix86 */
#if !defined(__i386__)
#error "This code needs to be compiled with a ix86-elf compiler"
#endif

/* Hardware text mode color constants */
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};
```

Compile with:
```bash
i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
```

## Linking

Link with:
```bash
i686-elf-gcc -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc
```

# Setup GRUB Bootloader

Check myos.bin with
```bash
grub-file --is-x86-multiboot myos.bin && echo multiboot OK || echo NOT multiboot
```

```bash
# create grub config file
cat << EOF > grub.cfg
menuentry "myos" {
	multiboot /boot/myos.bin
}
EOF

# make a bootable image
mkdir -p isodir/boot/grub
cp myos.bin isodir/boot/myos.bin
cp grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o myos.iso isodir
```

# Virtualisation with QEMU

```bash
# Boot with
qemu-system-i386 -cdrom myos.iso
# or directly with
qemu-system-i386 -kernel myos.bin
```

# Debugging with KVM

```bash
# launch kvm
kvm -m 2048 -s hda myos.iso -redir tcp:2323::23 -curses

# debug kernel in gdb
(gdb) target remote:1234
```

# Booting from a USB Stick

```bash
USB_BLOCK_DEVICE=/dev/sdx
sudo dd if=myos.iso of=$USB_BLOCK_DEVICE && sync
```

# References

* [OSdev Bare Bones](https://wiki.osdev.org/Bare_Bones)
* [GCC cross compiler](https://wiki.osdev.org/GCC_Cross-Compiler)
* [KVM on headless ubuntu](https://www.cyberciti.biz/faq/installing-kvm-on-ubuntu-16-04-lts-server/)
