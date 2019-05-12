section .text
global keyboard_int
extern keyboard_handler

keyboard_int:
	call    keyboard_handler
	iretd
