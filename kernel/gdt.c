/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gdt.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/12 12:05:46 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/05 00:26:41 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "kfs_stdint.h"
#include "gdt.h"

struct gdt_entry	gdt_entries[] =
{
	/* Entry Zero */
	{0, 0, 0, {0}, {0}, 0},
	/* Kernel code segment */
	{
		.limit_low   = 0xffff,
		.base_low    = 0,
		.base_middle = 0,
		.base_high   = 0,
		.access.data = (GDT_ACCESS_RW | GDT_ACCESS_EX | GDT_ACCESS_P |
				GDT_ACCESS_RING(GDT_PVL_KERNEL) |
				GDT_ACCESS_DT),
		.gran.data   = (GDT_GRAN_32BIT | GDT_GRAN_EXPND | 0xf)
	},
	/* Kernel data segment */
	{
		.limit_low   = 0xffff,
		.base_low    = 0,
		.base_middle = 0,
		.base_high   = 0,
		.access.data = (GDT_ACCESS_RW | GDT_ACCESS_P |
				GDT_ACCESS_RING(GDT_PVL_KERNEL) |
				GDT_ACCESS_DT),
		.gran.data   = (GDT_GRAN_32BIT | GDT_GRAN_EXPND | 0xf)
	},
	/* Kernel stack segment, reversed */
	{
		.limit_low   = 0xffff,
		.base_low    = 0,
		.base_middle = 0,
		.base_high   = 0,
		.access.data = (GDT_ACCESS_RW | GDT_ACCESS_P |
				GDT_ACCESS_RING(GDT_PVL_KERNEL) |
				GDT_ACCESS_DC | GDT_ACCESS_DT),
		.gran.data   = (GDT_GRAN_32BIT | GDT_GRAN_EXPND | 0xf)
	},
	/* User code segment */
	{
		.limit_low   = 0xffff,
		.base_low    = 0,
		.base_middle = 0,
		.base_high   = 0,
		.access.data = (GDT_ACCESS_RW | GDT_ACCESS_EX | GDT_ACCESS_P |
				GDT_ACCESS_RING(GDT_PVL_USER) |
				GDT_ACCESS_DT),
		.gran.data   = (GDT_GRAN_32BIT | GDT_GRAN_EXPND | 0xf),
	},
	/* User data segment */
	{
		.limit_low   = 0xffff,
		.base_low    = 0,
		.base_middle = 0,
		.base_high   = 0,
		.access.data = (GDT_ACCESS_RW | GDT_ACCESS_P |
				GDT_ACCESS_RING(GDT_PVL_USER) |
				GDT_ACCESS_DT),
		.gran.data   = (GDT_GRAN_32BIT | GDT_GRAN_EXPND | 0xf),
	},
	/* User stack segment, reversed */
	{
		.limit_low   = 0xffff,
		.base_low    = 0,
		.base_middle = 0,
		.base_high   = 0,
		.access.data = (GDT_ACCESS_RW | GDT_ACCESS_P |
				GDT_ACCESS_RING(GDT_PVL_USER) |
				GDT_ACCESS_DC | GDT_ACCESS_DT),
		.gran.data   = (GDT_GRAN_32BIT | GDT_GRAN_EXPND | 0xf)
	},
};

struct gdt_ptr		gdtp =
{
	.limit = sizeof(gdt_entries) - 1,
	.base  = (u32)gdt_entries
};

void			gdt_init(void)
{
	gdt_flush(&gdtp);
}
