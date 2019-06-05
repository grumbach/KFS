/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/07 00:00:38 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/06 00:33:38 by agrumbac         ###   ########.fr       */
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

	keyboard_init();
	printk("[LOG] keyboard initialised!\n");
}
