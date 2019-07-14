/*
 *  This file is part of Kernel From Scratch.
 *
 *	Kernel From Scratch is free software: you can redistribute it and/or modify
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

#include "private_libkfs.h"

int		strcmp(const char *s1, const char *s2)
{
	const unsigned char *ss1;
	const unsigned char *ss2;
	unsigned char c1;
	unsigned char c2;

	ss1 = (const unsigned char *)s1;
	ss2 = (const unsigned char *)s2;
	c1 = *ss1;
	c2 = *ss2;
	while (c1 == c2)
	{
		if (c1 == 0)
			return (c1 - c2);
		c1 = *++ss1;
		c2 = *++ss2;
	}
	return (c1 - c2);
}
