/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/07 00:00:38 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/07 00:54:00 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "kernel.h"

void	kernel_main(void)
{
	terminal_init();
	printk("[LOG] TERMINAL initialised!\n");

	gdt_init();
	printk("[LOG] GDT initialised!\n");

	idt_init();
	printk("[LOG] IDT initialised!\n");

	pic_init();
	printk("[LOG] PIC initialised!\n");

	while (42)
		keyboard_handler();
}
