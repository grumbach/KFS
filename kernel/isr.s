; **************************************************************************** ;
;                                                                              ;
;                                                         :::      ::::::::    ;
;    isr.s                                              :+:      :+:    :+:    ;
;                                                     +:+ +:+         +:+      ;
;    By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+         ;
;                                                 +#+#+#+#+#+   +#+            ;
;    Created: 2019/06/05 22:39:01 by agrumbac          #+#    #+#              ;
;    Updated: 2019/06/05 22:53:45 by agrumbac         ###   ########.fr        ;
;                                                                              ;
; **************************************************************************** ;

;; AMD64 Volume 2 8.9.3
;; Handles the EFLAGS.IF (interrupt flag)
;; bit according to the gate-descriptor type :
;;  - If the gate descriptor is an interrupt gate, EFLAGS.IF is cleared to 0.
;;  - If the gate descriptor is a trap gate, EFLAGS.IF is not modified.
;; Our ISRs are interrupt gates no need for cli/sti instruction

%macro noerr 1
global _isr%1
	_isr%1:
	push DWORD 0
	push DWORD %1
	jmp isr_common_stub
%endmacro

%macro err 1
global _isr%1
	_isr%1:
	push DWORD %1
	jmp isr_common_stub
%endmacro

;; Interrupt Service Routines

	noerr	0
	noerr	1
	noerr	2
	noerr	3
	noerr	4
	noerr	5
	noerr	6
	noerr	7
	err	8
	noerr	9
	err	10
	err	11
	err	12
	err	13
	err	14
	noerr	16
	err	17
	noerr	18
	noerr	19
	noerr	20

global _undefined
_undefined:
	push DWORD 0
	push DWORD -1
	jmp isr_common_stub

extern isr_fault_handler

isr_common_stub:
	pushad              ; general registers
	push ds             ; save original selectors
	push es
	push fs
	push gs
	push ss

	mov ax, 0x10        ; set kernel selectors
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ax, 0x18
	mov ss, ax

	call isr_fault_handler

	pop ss              ; restore original selectors
	pop gs
	pop fs
	pop es
	pop ds
	popad

;; IA32E Intel Volume 3A 6.13
;; Note that the error code is not popped when the IRET instruction is
;; executed to return from an exception handler, so the handler must
;; remove the error code before executing a return.
	add esp, 8
	iretd
