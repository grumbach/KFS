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

size_t		strlen(const char *str)
{
	OP		*long_ptr;
	OP		val;
	char	*cp;

	cp = (char *)str;
	while ((OP)cp & OP_MASK) {
		if (*cp == 0)
			return (cp - str);
		++cp;
	}
	long_ptr = (OP *)cp;
	for (;;) {
		val = *long_ptr;
		if ((val - LBITS) & HBITS) {
			cp = (char *)long_ptr;
			if (*cp == 0)
				return (cp - str);
			if (*++cp == 0)
				return (cp - str);
			if (*++cp == 0)
				return (cp - str);
			if (*++cp == 0)
				return (cp - str);
#if __x86_64__
			if (*++cp == 0)
				return (cp - str);
			if (*++cp == 0)
				return (cp - str);
			if (*++cp == 0)
				return (cp - str);
			if (*++cp == 0)
				return (cp - str);
#endif
		}
		++long_ptr;
	}
}
