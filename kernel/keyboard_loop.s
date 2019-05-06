section text

global keyboard_loop
global crash_please
extern terminal_putchar

keyboard_loop:
	mov eax, 0x0
	int 16
	push eax
	call terminal_putchar
	pop eax
	ret

crash_please:
	int 0x16
