/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/12 16:03:17 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/06 00:29:56 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KERNEL_H
# define KERNEL_H

# include <stdbool.h>
# include <stdint.h>
# include "libkfs.h"

/*
** kernel
*/

void		gdt_init(void);
void		idt_init(void);
void		terminal_init(void);
void		keyboard_init(void);

/*
** utils
*/

void		sleep(size_t time);
size_t		strlen(const char *str);

#endif
