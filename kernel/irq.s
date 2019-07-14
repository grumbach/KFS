; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    irq.s                                              :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2019/06/07 00:27:50 by agrumbac          #+#    #+#              ;
;    Updated: 2019/06/07 00:28:42 by agrumbac         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

extern keyboard_handler

global _irq1
_irq1:
	pushad
	call keyboard_handler
	popad
	iret

extern terminal_putchar

global _irq0
_irq0:
	pushad

	push DWORD 42
	call terminal_putchar
	add esp, 4

	mov ax, 0x20
	mov dx, 0x20
	out dx, ax

	popad

	iret
