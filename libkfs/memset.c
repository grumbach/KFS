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

static inline void	memset64(OP dstp, int c, size_t m8)
{
	register size_t		xlen;
	register OP			mask_set;

	mask_set = c << 8 | c;
	mask_set = (mask_set << 16) | mask_set;
#if __x86_64__
	mask_set = ((mask_set << 16) << 16) | mask_set;
#endif
	xlen = m8 >> OP_SHIFT;
	while (xlen--) {
		((OP *)dstp)[0] = mask_set;
		((OP *)dstp)[1] = mask_set;
		((OP *)dstp)[2] = mask_set;
		((OP *)dstp)[3] = mask_set;
		((OP *)dstp)[4] = mask_set;
		((OP *)dstp)[5] = mask_set;
		((OP *)dstp)[6] = mask_set;
		((OP *)dstp)[7] = mask_set;
		dstp += (OP_SIZE << 3);
	}
	m8 &= 7;
	while (m8--) {
		((OP *)dstp)[0] = mask_set;
		dstp += OP_SIZE;
	}
}

static inline void	memset8(OP dstp, int c, size_t len)
{
	while (len--)
		((u8 *)dstp++)[0] = c;
}

void				*memset(void *mem, int c, size_t mlen)
{
	OP		dstp;
	size_t	xlen;

	dstp = (OP)mem;
	c &= 0xff;
	if (mlen >= (OP_SIZE << 1)) {
		xlen = -dstp & OP_MASK;
		memset8(dstp, c, xlen);
		mlen -= xlen;
		dstp += xlen;
		xlen = mlen >> OP_SHIFT;
		memset64(dstp, c, xlen);
		dstp += (xlen << OP_SHIFT);
		mlen &= OP_MASK;
	}
	memset8(dstp, c, mlen);
	return (mem);
}
