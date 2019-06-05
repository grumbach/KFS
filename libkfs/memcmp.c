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

static int	memcmp8(OP s1p, OP s2p, size_t n)
{
	register u8	s1c;
	register u8	s2c;

	while (n--) {
		s1c = ((u8 *)s1p++)[0];
		s2c = ((u8 *)s2p++)[0];
		if (s2c != s1c)
			return (s1c - s2c);
	}
	return (0);
}

static int	_unaligned_word_cmp(OP s1p, OP s2p, size_t m8)
{
	OP		val;
	size_t	xlen;
	OP		w[2];
	int		sh[2];

	xlen = m8 >> 1;
	sh[0] = (s2p & OP_MASK) << OP_SHIFT;
	sh[1] = OP_WIDTH - sh[0];
	w[0] = ((OP *)s2p)[0];
	while (xlen--) {
		w[1] = ((OP *)s2p)[1];
		if (((val = MERGE(w[0], sh[0], w[1], sh[1])) ^ ((OP *)s1p)[0]))
			return (memcmp8(s1p, (OP)&val, OP_SIZE));
		s2p += (OP_SIZE << 1);
		w[0] = ((OP *)s2p)[0];
		if (((val = MERGE(w[1], sh[0], w[0], sh[1])) ^ ((OP *)s1p)[1]))
			return (memcmp8(s1p + OP_SIZE, (OP)&val, OP_SIZE));
		s1p += (OP_SIZE << 1);
	}
	w[1] = ((OP *)s2p)[1];
	if (m8 & 1)
		if (!((val = MERGE(w[0], sh[0], w[1], sh[1])) ^ ((OP *)s1p)[0]))
			return (memcmp8(s1p, (OP)&val, OP_SIZE));
	return (0);
}

static int	_aligned_word_cmp(OP s1p, OP s2p, size_t m8)
{
	size_t	xlen;

	xlen = m8 >> 3;
	while (xlen--) {
		if ((((OP *)s2p)[0] ^ ((OP *)s1p)[0]))
			return (memcmp8(s1p, s2p, OP_SIZE));
		if ((((OP *)s2p)[1] ^ ((OP *)s1p)[1]))
			return (memcmp8(s1p + OP_SIZE, s2p + OP_SIZE, OP_SIZE));
		if ((((OP *)s2p)[2] ^ ((OP *)s1p)[2]))
			return (memcmp8(s1p + OP_SIZE * 2, s2p + OP_SIZE * 2, OP_SIZE));
		if ((((OP *)s2p)[3] ^ ((OP *)s1p)[3]))
			return (memcmp8(s1p + OP_SIZE * 3, s2p + OP_SIZE * 3, OP_SIZE));
		s1p += (OP_SIZE << 2);
		s2p += (OP_SIZE << 2);
	}
	m8 &= 3;
	while (m8--) {
		if ((((OP *)s2p)[0] ^ ((OP *)s1p)[0]))
			return (memcmp8(s1p, s2p, OP_SIZE));
		s1p += OP_SIZE;
		s2p += OP_SIZE;
	}
	return (0);
}

int			memcmp(const void *s1, const void *s2, size_t n)
{
	OP		s1p;
	OP		s2p;
	size_t	xlen;
	int		diff;

	s1p = (OP)s1;
	s2p = (OP)s2;
	if (n >= (OP_SIZE << 1)) {
		xlen = -s1p & OP_MASK;
		if ((diff = memcmp8(s1p, s2p, xlen)))
			return (diff);
		n -= xlen;
		s1p += xlen;
		s2p += xlen;
		xlen = n >> OP_SHIFT;
		diff = s2p & OP_MASK ?
			_unaligned_word_cmp(s1p, s2p & ~OP_MASK, xlen) :
			_aligned_word_cmp(s1p, s2p, xlen);
		if (diff)
			return (diff);
		s1p += (xlen << OP_SHIFT);
		s2p += (xlen << OP_SHIFT);
		n &= OP_MASK;
	}
	return (memcmp8(s1p, s2p, n));
}
