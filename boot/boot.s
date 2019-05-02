; multiboot constants
%define ALIGN     1<<0             ; align loaded modules on page boundaries
%define MEMINFO   1<<1             ; provide memory map
%define FLAGS     (ALIGN | MEMINFO); this is the Multiboot 'flag' field
%define MAGIC     0x1BADB002       ; 'magic number' lets bootloader find the header
%define CHECKSUM  -(MAGIC + FLAGS) ; checksum of above, to prove we are multiboot

; multiboot header
bits 32
section .multiboot
align 4
dd MAGIC
dd FLAGS
dd CHECKSUM

; create some space for stack
section .bss
align 16
stack_bottom:
resb 16384                         ; 16 KiB
stack_top:

; kernel start
section text

global _start
extern kernel_main

_start:
    mov esp, stack_top             ; setup stack
                                   ; load GDT, enable paging...
    call kernel_main               ; enter high-level kernel
    cli                            ; disable interrupts
infinite_loop:
    hlt                            ; halt forever
    jmp infinite_loop

; set _start size for debug
;size _start, . - _start           ; TODO nasm syntax?

