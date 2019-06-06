/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pic.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/07 00:29:13 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/07 00:46:03 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libkfs.h"

extern u16	read_port(u16 port);
extern void	write_port(u16 port, u16 cmd);

void	pic_init(void)
{
	/* ICW1 - begin initialization */
	write_port(0x20 , 0x11);
	write_port(0xA0 , 0x11);

	/* ICW2 - remap offset address of IDT */
	write_port(0x21 , 0x20);
	write_port(0xA1 , 0x28);

	/* ICW3 - setup cascading */
	write_port(0x21 , 0x04);
	write_port(0xA1 , 0x02);

	/* ICW4 - environment info */
	write_port(0x21 , 0x01);
	write_port(0xA1 , 0x01);

	/* Initialization finished */
	/* mask interrupts */
	/* 0xfd is 11111100 - enables only IRQ1 (keyboard) and IRQ0 */
	write_port(0x21 , 0xfc);
	write_port(0xA1 , 0xff);
}
