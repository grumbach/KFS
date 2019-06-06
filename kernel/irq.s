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
	call keyboard_handler
	sti
	iret

extern terminal_putchar

global _irq0
_irq0:
	push DWORD 42
	call terminal_putchar
	add esp, 4
	sti
	iret
