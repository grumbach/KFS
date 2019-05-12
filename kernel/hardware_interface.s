section .text

; --------- ports ---------
global read_port
global write_port

read_port:
	mov edx, [esp + 4]
	in al, dx
	ret

write_port:
	mov   edx, [esp + 4]
	mov   al, [esp + 4 + 4]
	out   dx, al
	ret

; ---------  gdt  ---------
global gdt_flush

gdt_flush:
	mov edx, [esp + 4]
	lgdt [edx]
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	jmp 0x08:new_flush
new_flush:
	ret

; ---------  idt  ---------
global idt_flush

idt_flush:
	mov edx, [esp + 4]
	lidt [edx]
	sti 			;turn on interrupts
	ret

; --------- crash ---------
global crash_please

crash_please:
	int 0x16
