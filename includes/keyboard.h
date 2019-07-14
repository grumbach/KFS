/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/05 00:29:20 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/05 00:33:22 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KEYBOARD_H
# define KEYBOARD_H

# include "libkfs.h"

# define PIC_BASE_PORT		0x20
# define PIC_EOI		0x20        /* End Of Interrupt               */

# define KEYBOARD_DATA_PORT	0x60
# define KEYBOARD_STATUS_PORT 	0x64

# define KEYCODE_RELEASE	0x80        /* shift, alt, or control keys... */

u16		read_port(u16 port);
void		write_port(u16 port, u16 cmd);

#endif
