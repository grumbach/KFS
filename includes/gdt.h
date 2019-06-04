/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gdt.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/04 23:00:54 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/05 00:27:40 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GDT_H
# define GDT_H

# include "kfs_stdint.h"

# define GDT_ACCESS_RW		0x02
# define GDT_ACCESS_DC		0x04
# define GDT_ACCESS_EX		0x08

# define GDT_ACCESS_DT		0x10
# define GDT_PVL_KERNEL		0
# define GDT_PVL_USER		3
# define GDT_ACCESS_RING(x)	(((x) & 3) << 5)
# define GDT_ACCESS_P		0x80

# define GDT_GRAN_32BIT		0x40
# define GDT_GRAN_EXPND		0x80

struct access
{
	u8			ac:1;          /* accessed by cpu            */
	u8			rw:1;          /* read write                 */
	u8			dc:1;          /* direction 0 = up, 1 = down */
	u8			ex:1;          /* 1 = executable, 0 = data   */
	u8			desc_type:1;   /* 0 = system, 1 = code|data  */
	u8			ring:2;        /* 0 = kernel, 3 = user       */
	u8			present:1;     /* 1 = segment is present     */
} __attribute__((packed));

union gdt_access_u
{
	u8			data;
	struct access		bits;
} __attribute__((packed));

struct granularity
{
	u8			seg_len:4;     /* bits 19:16 of limit      */
	u8			available:1;   /* Available for System (0) */
	u8			zero:1;        /* Always 0                 */
	u8			op_size:1;     /* 0 = 16bit, 1 = 32-bit    */
	u8			granularity:1; /* 0 = 1byte, 1 = 4kbyte    */
} __attribute__((packed));

union gdt_granularity_u
{
	u8			data;
	struct granularity	bits;

} __attribute__((packed));

struct gdt_entry
{
	u16			limit_low;
	u16			base_low;
	u8			base_middle;
	union gdt_access_u	access;
	union gdt_granularity_u	gran;
	u8			base_high;
} __attribute__((packed));

struct gdt_ptr
{
	u16			limit;
	u32			base;
} __attribute__((packed));

void			gdt_flush(struct gdt_ptr *gdtp);

#endif
