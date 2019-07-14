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

static void	_word_copy_dest_aligned(OP dstp, OP srcp, size_t m8)
{
	OP		w[2];
	size_t	xlen;
	int		sh[2];

	xlen = m8 >> 1;
	sh[0] = (srcp & OP_MASK) * 8;
	sh[1] = OP_WIDTH - sh[0];
	srcp -= OP_SIZE;
	dstp -= (OP_SIZE << 1);
	srcp &= ~OP_MASK;
	w[1] = ((OP *)srcp)[1];
	while (xlen--) {
		w[0] = ((OP *)srcp)[0];
		((OP *)dstp)[1] = MERGE(w[0], sh[0], w[1], sh[1]);
		srcp -= (OP_SIZE << 1);
		w[1] = ((OP *)srcp)[1];
		((OP *)dstp)[0] = MERGE(w[1], sh[0], w[0], sh[1]);
		dstp -= (OP_SIZE << 1);
	}
	if (m8 & 1) {
		w[0] = ((OP *)srcp)[0];
		((OP *)dstp)[1] = MERGE(w[0], sh[0], w[1], sh[1]);
	}
}

static void	_word_copy_aligned(OP dstp, OP srcp, size_t m8)
{
	OP		w;
	size_t	xlen;

	xlen = m8 >> 2;
	while (xlen--) {
		dstp -= (OP_SIZE << 2);
		srcp -= (OP_SIZE << 2);
		w = ((OP *)srcp)[3];
		((OP *)dstp)[3] = w;
		w = ((OP *)srcp)[2];
		((OP *)dstp)[2] = w;
		w = ((OP *)srcp)[1];
		((OP *)dstp)[1] = w;
		w = ((OP *)srcp)[0];
		((OP *)dstp)[0] = w;
	}
	m8 &= 3;
	while (m8--) {
		dstp -= OP_SIZE;
		srcp -= OP_SIZE;
		w = ((OP *)srcp)[0];
		((OP *)dstp)[0] = w;
	}
}

static void	_byte_copy_bwd(OP dst, OP src, size_t n)
{
	while (n--)
		((u8 *)--dst)[0] = ((u8 *)--src)[0];
}

static void	memcpy_bwd(OP dstp, OP srcp, size_t n)
{
	size_t	xlen;

	dstp += n;
	srcp += n;
	if (n >= (OP_SIZE << 1)) {
		xlen = dstp & OP_MASK;
		_byte_copy_bwd(dstp, srcp, xlen);
		dstp -= xlen;
		srcp -= xlen;
		n -= xlen;
		xlen = n >> OP_SHIFT;
		if (srcp & OP_MASK)
			_word_copy_dest_aligned(dstp, srcp, xlen);
		else
			_word_copy_aligned(dstp, srcp, xlen);
		dstp -= (xlen << OP_SHIFT);
		srcp -= (xlen << OP_SHIFT);
		n &= OP_MASK;
	}
	_byte_copy_bwd(dstp, srcp, n);
}

void		*memmove(void *dst, const void *src, size_t n)
{
	OP		dstp;
	OP		srcp;

	dstp = (OP)dst;
	srcp = (OP)src;
	if (dstp - srcp >= n)
		memcpy(dst, src, n);
	else
		memcpy_bwd(dstp, srcp, n);
	return (dst);
}
