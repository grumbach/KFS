/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/03 19:49:48 by agrumbac          #+#    #+#             */
/*   Updated: 2019/05/12 16:24:32 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "kernel.h"

#define VGA_BUFFER		0xb8000
#define VGA_WIDTH		80
#define VGA_HEIGHT		25

enum vga_color
{
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

static struct
{
	uint16_t	(*buffer)[VGA_HEIGHT][VGA_WIDTH];
	size_t		row;
	size_t		column;
	uint8_t		color;
}			term = {.buffer = (void *)VGA_BUFFER};

static inline uint8_t	vga_entry_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}

static inline uint16_t	vga_entry(unsigned char uc, uint8_t color)
{
	return (uint16_t) uc | (uint16_t) color << 8;
}

void			terminal_init(void)
{
	term.buffer = (void *)VGA_BUFFER;
	term.row = 0;
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
	if (c == 0) return;

	if (c == '\n')
	{
		term.column = 0;
		term.row++;
	} else {
		(*term.buffer)[term.row][term.column++] = vga_entry(c, term.color);
	}

	if (term.column == VGA_HEIGHT)
	{
		term.column = 0;
		term.row++;
	}
	if (term.row == VGA_HEIGHT) term.row = 0;
}

void			terminal_putstr(char *str)
{
	while (*str)
	{
		terminal_putchar(*str);
		str++;
	}
}
