/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/12 16:03:17 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/07 00:47:40 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KERNEL_H
# define KERNEL_H

# include <stdbool.h>
# include <stdint.h>
# include "libkfs.h"

/*
** kernel init
*/

void		gdt_init(void);
void		idt_init(void);
void		pic_init(void);
void		terminal_init(void);

#endif
