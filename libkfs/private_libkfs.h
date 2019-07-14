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

#ifndef __PRIVATE_LIBKFS_H__
# define __PRIVATE_LIBKFS_H__

# include "../includes/libkfs.h"

# if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#  define MERGE(x1, sh1, x2, sh2) (((x1) << (sh1)) | ((x2) >> (sh2)))
# else
#  define MERGE(x1, sh1, x2, sh2) (((x1) >> (sh1)) | ((x2) << (sh2)))
# endif

# if __x86_64__

#  define LBITS 0x101010101010101L
#  define HBITS 0x8080808080808080L

#  define size_t __UINT_LEAST64_TYPE__
#  define OP __UINT_LEAST64_TYPE__
#   if defined(__clang__)
#    define OP_WIDTH (sizeof(__UINT_LEAST64_TYPE__) * 8)
#   elif defined(__GNUC__)
#    define OP_WIDTH __INT_LEAST64_WIDTH__
#  endif
#  define OP_SHIFT 3
# else

#  define LBITS 0x1010101L
#  define HBITS 0x80808080L

#  define size_t __UINT_LEAST32_TYPE__
#  define OP __UINT_LEAST32_TYPE__
#   if defined(__clang__)
#    define OP_WIDTH (sizeof(__UINT_LEAST32_TYPE__) * 8)
#   elif defined(__GNUC__)
#    define OP_WIDTH __INT_LEAST32_WIDTH__
#  endif
#  define OP_SHIFT 2
# endif  /* arch check  */

# define u8 __UINT_LEAST8_TYPE__
# define OP_SIZE sizeof(OP)
# define OP_MASK (OP_SIZE - 1)

# define is_digit(c) (0x30 <= (c) && 0x39 >= (c))

void	*memset(void *, int, size_t);
void	*memcpy(void *, const void *, size_t);
void	*memmove(void *, const void *, size_t);
void	*memchr(const void *, int, size_t);
int	memcmp(const void *, const void *, size_t);

size_t	strlen(const char *);
size_t	strnlen(const char *, size_t);
char	*strcpy(char *, const char *);
char	*strchr(const char *, int);
int	strcmp(const char *, const char *);

#endif /* __PRIVATE_LIBKFS_H__ */
