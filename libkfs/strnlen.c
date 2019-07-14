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

size_t		strnlen(const char *str, size_t n)
{
	OP			*long_ptr;
	OP			val;
	const char	*cp;
	const char	*end = str + n;

	if (n == 0)
		return 0;

	if (__builtin_expect(end < str, 0))
		end = (char *)~0UL;

	cp = (char *)str;

	while ((OP)cp & OP_MASK) {
		if (*cp == 0)
			goto end;
		++cp;
	}
	long_ptr = (OP *)cp;
	val = *long_ptr;
	for (;(char *)long_ptr < end;
	     val = *++long_ptr, cp = (char *)long_ptr) {
		if ((val - LBITS) & HBITS) {
			if (*cp == 0)
				break ;
			if (*++cp == 0)
				break ;
			if (*++cp == 0)
				break ;
			if (*++cp == 0)
				break ;
#if __x86_64__
			if (*++cp == 0)
				break ;
			if (*++cp == 0)
				break ;
			if (*++cp == 0)
				break ;
			if (*++cp == 0)
				break ;
#endif
		}
	}
end:
	if (cp > end)
		cp = end;
	return (cp - str);
}
