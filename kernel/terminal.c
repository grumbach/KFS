/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/03 19:49:48 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/06 00:47:53 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "kernel.h"
#include "terminal.h"
#include "io.h"

#define VGA_BUFFER		((uint16_t *)0xb8000)
#define VGA_WIDTH		80
#define VGA_HEIGHT		25

static struct
{
	uint16_t	(*buffer)[VGA_HEIGHT][VGA_WIDTH];
	size_t		row;
	size_t		column;
	uint8_t		color;
}			term = {.buffer = (uint16_t (*)[VGA_HEIGHT][VGA_WIDTH])VGA_BUFFER};

static inline uint8_t	vga_entry_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}

static inline uint16_t	vga_entry(unsigned char uc, uint8_t color)
{
	return (uint16_t) uc | (uint16_t) color << 8;
}

void			update_curse(void)
{
	uint16_t cursorLocation =  term.row * VGA_WIDTH + term.column + 1;

	outb(0x3D4, 14);                  // Tell the VGA board we are setting the high cursor byte.
	outb(0x3D5, cursorLocation >> 8); // Send the high cursor byte.
	outb(0x3D4, 15);                  // Tell the VGA board we are setting the low cursor byte.
	outb(0x3D5, cursorLocation);      // Send the low cursor byte.
}

void			terminal_init(void)
{
	term.buffer = (void *)VGA_BUFFER;
	term.row = VGA_HEIGHT - 1;
	term.column = 0;
	term.color = vga_entry_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);

	for (size_t i = 0; i < VGA_HEIGHT; i++)
	{
		for (size_t j = 0; j < VGA_WIDTH; j++)
			(*term.buffer)[i][j] = vga_entry(' ', term.color);
	}
}

void			terminal_putchar(unsigned char c)
{
	if (c == 0)
		return;

	if (c == '\n') {
		term.column = 0;
		term.row++;
	} else {
		(*term.buffer)[term.row][term.column++] = vga_entry(c, term.color);
	}

	if (term.column >= VGA_WIDTH) {
		term.column = 0;
		term.row++;
	}
	
	if (term.row >= VGA_HEIGHT) {
		term.row -= 1;
		memmove(VGA_BUFFER,
			VGA_BUFFER + VGA_WIDTH,
			(VGA_HEIGHT - 1) * VGA_WIDTH * sizeof(uint16_t));

		memset(VGA_BUFFER + (VGA_HEIGHT - 1) * VGA_WIDTH,
		       0,
		       VGA_WIDTH * sizeof(uint16_t));
	}
	update_curse();
}

void			terminal_putstr(const char *str)
{
	while (*str)
	{
		terminal_putchar(*str);
		str++;
	}
}

void			terminal_write(const char *mem, size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		terminal_putchar(mem[i]);
	}
}
