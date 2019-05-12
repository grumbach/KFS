/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gdt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/12 12:05:46 by agrumbac          #+#    #+#             */
/*   Updated: 2019/05/12 17:18:32 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "kernel.h"

# define SEG_DATA_RDWR		0x02 /* Read, Write              */
# define SEG_DATA_RDWREXPD	0x06 /* Read, Write, expand-down */
# define SEG_CODE_EXRD		0x0A /* Execute, Read            */

struct access
{
	unsigned char		type:4;        /* which type rwx, direction */
	unsigned char		desc_type:1;   /* 0 = system, 1 = code|data */
	unsigned char		ring:2;        /* 0 = kernel, 3 = user      */
	unsigned char		present:1;     /* 1 = segment is present    */
} __attribute__((packed));

struct granularity
{
	unsigned char		seg_len:4;     /* bits 19:16 of limit      */
	unsigned char		available:1;   /* Available for System (0) */
	unsigned char		zero:1;        /* Always 0                 */
	unsigned char		op_size:1;     /* 0 = 16bit, 1 = 32-bit    */
	unsigned char		granularity:1; /* 0 = 1byte, 1 = 4kbyte    */
} __attribute__((packed));

struct gdt_entry
{
	unsigned short		limit_low;
	unsigned short		base_low;
	unsigned char		base_middle;
	struct access		access;
	struct granularity	granularity;
	unsigned char		base_high;
} __attribute__((packed));

struct gdt_entry		gdt[7];
struct gdt_ptr			gdtp;

static void	gdt_set_gate(int num, unsigned long base, unsigned long limit, \
			struct access acc, struct granularity gran)
{
	/* Setup the descriptor base address */
	gdt[num].base_low                 = (base & 0xffff);
	gdt[num].base_middle              = (base >> 16) & 0xff;
	gdt[num].base_high                = (base >> 24) & 0xff;

	/* Setup the descriptor limits */
	gdt[num].limit_low                = (limit & 0xffff);
	gdt[num].granularity.seg_len      = ((limit >> 16) & 0x0f);

	/* Finally, set up the granularity and access flags */
	gdt[num].granularity.available   |= gran.available;
	gdt[num].granularity.zero        |= gran.zero;
	gdt[num].granularity.op_size     |= gran.op_size;
	gdt[num].granularity.granularity |= gran.granularity;
	gdt[num].access                   = acc;
}

void		gdt_init(void)
{
	/* NULL descriptor */
	gdt_set_gate(0, 0, 0, (struct access){.type = 0}, \
		(struct granularity){.zero = 0});

	/* Kernel Code  */
	gdt_set_gate(1, 0, 0xffffffff, \
		(struct access){.present = 1, .desc_type = 1, .type = SEG_CODE_EXRD}, \
		(struct granularity){.op_size = 1, .granularity = 1});

	/* Kernel Data  */
	gdt_set_gate(2, 0, 0xffffffff, \
		(struct access){.present = 1, .desc_type = 1, .type = SEG_DATA_RDWR}, \
		(struct granularity){.op_size = 1, .granularity = 1});

	/* Kernel stack */
	gdt_set_gate(3, 0, 0xffffffff, \
		(struct access){.present = 1, .desc_type = 1, .type = SEG_DATA_RDWREXPD}, \
		(struct granularity){.op_size = 1, .granularity = 1});

	/* User code    */
	gdt_set_gate(4, 0, 0xffffffff, \
		(struct access){.present = 1, .ring = 3, .desc_type = 1, .type = SEG_CODE_EXRD}, \
		(struct granularity){.op_size = 1, .granularity = 1});

	/* User data    */
	gdt_set_gate(5, 0, 0xffffffff, \
		(struct access){.present = 1, .ring = 3, .desc_type = 1, .type = SEG_DATA_RDWR}, \
		(struct granularity){.op_size = 1, .granularity = 1});

	/* User stack   */
	gdt_set_gate(6, 0, 0xffffffff, \
		(struct access){.present = 1, .ring = 3, .desc_type = 1, .type = SEG_DATA_RDWREXPD}, \
		(struct granularity){.op_size = 1, .granularity = 1});

	gdtp.limit = (sizeof(gdt)) - 1;
	gdtp.base = (unsigned int)&gdt;

	gdt_flush(gdtp);
}
