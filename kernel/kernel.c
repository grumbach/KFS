/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/07 00:00:38 by agrumbac          #+#    #+#             */
/*   Updated: 2019/05/12 17:27:14 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "kernel.h"

void kernel_main(void)
{
	terminal_init();
	terminal_putstr("[LOG] Hello, kernel World!\n");
	sleep(3);

	gdt_init();
	terminal_putstr("[LOG] GDT initialised!\n");
	sleep(3);

	// idt_init();
	// terminal_putstr("[LOG] IDT initialised!\n");
	// sleep(3);

	keyboard_init();

	sleep(10);

	crash_please();
}
