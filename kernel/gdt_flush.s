; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    gdt_flush.s                                        :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2019/06/05 22:10:43 by agrumbac          #+#    #+#              ;
;    Updated: 2019/06/05 22:10:50 by agrumbac         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

section .text
global gdt_flush

gdt_flush:
	mov edx, [esp + 4]
	lgdt [edx]
	mov ax, 0x10        ; 0x10 points at the new data selector
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ax, 0x18        ; 0x18 points at the new stack selector
	mov ss, ax
	jmp 0x08:reload_cs  ; 0x08 points at the new code selector
	reload_cs:
	ret
