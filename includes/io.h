/*
 *  This file is part of Kernel From Scratch.
 *
 *  Kernel From Scratch is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef __IO_H__
# define __IO_H__ 1

# include "libkfs.h"

# define asminb(port, data)			\
	__asm__ __volatile__("inb %w1, %b0"	\
			     : "=a" (data)	\
			     : "d" (port))
# define outb(port, data)					\
	__asm__ __volatile__("outb %b0, %w1"			\
			     :: "a" (data), "d" (port))

__attribute__((always_inline))
static inline u8
inb(u8 port)
{
	u8 data;

	asminb(port, data);
	return data;
}

#endif /*__IO_H__*/
