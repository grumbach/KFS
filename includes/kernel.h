/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kernel.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/12 16:03:17 by agrumbac          #+#    #+#             */
/*   Updated: 2019/05/12 16:26:25 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KERNEL_H
# define KERNEL_H

# include <stdbool.h>
# include <stddef.h>
# include <stdint.h>

struct gdt_ptr
{
	unsigned short		limit;
	unsigned int		base;
} __attribute__((packed));

struct idt_ptr
{
	/* TODO */
} __attribute__((packed));

/*
** hardware interface
*/

uint16_t	read_port(uint16_t);
void		write_port(uint16_t,uint16_t);
void		gdt_flush(struct gdt_ptr gdtp);
void		idt_flush(struct idt_ptr idtp);
void		crash_please(void);

/*
** gdt idt
*/

void		gdt_init(void);
// void		idt_init(void);

/*
** keyboard
*/

void		keyboard_init(void);
void		keyboard_handler(void);

/*
** terminal
*/

void		terminal_init(void);
void		terminal_putchar(unsigned char c);
void		terminal_putstr(char *str);

/*
** utils
*/

void		sleep(size_t time);
size_t		strlen(const char *str);

#endif
