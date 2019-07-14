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

char	*strchr(const char *str, int c)
{
	register OP	mask;
	OP		*long_ptr;
	OP		val;
	u8		*cp;

	cp = (u8 *)str;
	while ((sizeof(OP) - 1) & (OP)cp)
	{
		if (*cp == (u8)c)
			return ((char *)cp);
		else if (*cp == 0)
			return (0);
		++cp;
	}
	mask = c & 0xff;
	mask = (mask << 8) | mask;
	mask = (mask << 16) | mask;
#if __x86_64__
	mask = ((mask << 16) << 16) | mask;
#endif
	long_ptr = (OP *)cp;
	for (;;)
	{
		val = *long_ptr;
		if (((val - LBITS) & HBITS) ||
		    (((val ^ mask) - LBITS) & HBITS)) {
			cp = (u8 *)long_ptr;
			if (*cp == (u8)c)
				return ((char *)cp);
			else if (*cp++ == 0)
				return (0);

			else if (*cp == (u8)c)
				return ((char *)cp);
			else if (*cp++ == 0)
				return (0);

			else if (*cp == (u8)c)
				return ((char *)cp);
			else if (*cp++ == 0)
				return (0);

			else if (*cp == (u8)c)
				return ((char *)cp);
			else if (*cp++ == 0)
				return (0);
#if __x86_64__
			else if (*cp == (u8)c)
				return ((char *)cp);
			else if (*cp++ == 0)
				return (0);

			else if (*cp == (u8)c)
				return ((char *)cp);
			else if (*cp++ == 0)
				return (0);

			else if (*cp == (u8)c)
				return ((char *)cp);
			else if (*cp++ == 0)
				return (0);

			else if (*cp == (u8)c)
				return ((char *)cp);
			else if (*cp++ == 0)
				return (0);
#endif

		}
		++long_ptr;
	}
}
