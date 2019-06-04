/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/07 00:00:38 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/05 00:11:22 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "kernel.h"

void kernel_main(void)
{
	terminal_init();
	terminal_putstr("[LOG] Hello, kernel World!\n");

	gdt_init();
	terminal_putstr("[LOG] GDT initialised!\n");

	keyboard_init();
}
