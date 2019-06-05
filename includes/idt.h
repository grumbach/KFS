/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   idt.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/05 22:08:12 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/05 23:10:03 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IDT_H
# define IDT_H

# include "libkfs.h"

/*
 * The following errors are defined by intel
 */

# define IDT_ISR_DIV		0  /* divide error */
# define IDT_ISR_DEBUG		1  /* debug exception */
# define IDT_ISR_NMI		2  /* nonmaskable interrupt */
# define IDT_ISR_BP		3  /* breakpoint exception */
# define IDT_ISR_OF		4  /* overflow */
# define IDT_ISR_BE		5  /* bound exceeded */
# define IDT_ISR_INVOP		6  /* invalid opcode */
# define IDT_ISR_DNA		7  /* device not available */
# define IDT_ISR_DF		8  /* double fault exception */
# define IDT_ISR_CSO		9  /* coprocessor segment overrun */
# define IDT_ISR_INVTSS		10 /* invalid tss exception */
# define IDT_ISR_SNP		11 /* segment not present */
# define IDT_ISR_SF		12 /* stack fault exception */
# define IDT_ISR_GP		13 /* general protection exception */
# define IDT_ISR_PF		14 /* page fault exception */
# define IDT_ISR_FP		16 /* flotion point error */
# define IDT_ISR_AC		17 /* alignment check exception */
# define IDT_ISR_MC		18 /* machine check exception */
# define IDT_ISR_SIMDFP		19 /* simd floating point exception */
# define IDT_ISR_VIRT		20 /* virtualization exception */

/*
 * 32-256 are user defined gate
 */

# define IDT_ISR_USR_GATE	32
# define IDT_ISR_USR_GATE_MAX	256

# define IDT_KERNEL_CS		0x8

/*
 * Gate type a described in Volume 3 Table 3-2 of the intel ia32 manual
 * below are 32/64 bit sized GATE type
 * no 16 (real mode) bit sized gate will be used
 */

# define IDT_GATE_TASK		0x5
# define IDT_GATE_TRAP		0xF
# define IDT_GATE_INT		0xE /* Interrupt */

# define IDT_PVL_KERNEL		0x0
# define IDT_PVL_USER		0x3

# define GATE(base, seg, ring, gate)			\
	{						\
		.base_low = ((base) & 0xffff),		\
		.base_high = ((base) >> 16) & 0xffff,	\
		.selector = (seg),			\
		.bits.zero = 0,				\
		.bits.ist = 0,				\
		.bits.type = (gate),			\
		.bits.dpl = (ring),			\
		.bits.p = 1,				\
	}

# define INTGATE(base, seg, ring)	GATE(base, seg, ring, IDT_GATE_INT)
# define TASKGATE(base, seg, ring)	GATE(base, seg, ring, IDT_GATE_TASK)
# define TRAPGATE(base, seg, ring)	GATE(base, seg, ring, IDT_GATE_TRAP)

struct idt_flag_bits
{
	u16			ist:3;          /* ist is unavailable in x86 */
	u16			zero:5;         /* always 0 */
	u16			type:5;         /* gate type */
	u16			dpl:2;          /* privilege ring */
	u16			p:1;            /* is present */
} __attribute__((packed));

struct idt_entry
{
	u16			base_low;       /* pointer to the function */
	u16			selector;       /* gdt selector */
	struct idt_flag_bits	bits;           /* flags */
	u16			base_high;      /* pointer to the function */
} __attribute__((packed));

struct idt_ptr
{
	u16			limit;
	u32			entp;
} __attribute__((packed));

struct	cpu_context
{
	u32	ss, gs ,fs, es, ds;                     /* segment selectors */
	u32	edi, esi, ebp, esp, ebx, edx, ecx, eax; /* general registers */
	u32	int_no, err_code;                       /* error info */
	u32	eip, cs, eflags;                        /* context */
	u32	useresp, userss;                        /* for context switch */
};

extern struct idt_entry	idt_entries[];
extern struct idt_ptr	idtp;

void			idt_flush(struct idt_ptr *idtp);

#endif
