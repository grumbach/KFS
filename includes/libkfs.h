/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libkfs.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/05 23:39:36 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/05 23:50:38 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFKS_H
# define LIBFKS_H

typedef __UINT_LEAST8_TYPE__	u8;
typedef __UINT_LEAST16_TYPE__	u16;
typedef __UINT_LEAST32_TYPE__	u32;
typedef __UINT_LEAST64_TYPE__	u64;

typedef __INT_LEAST8_TYPE__	s8;
typedef __INT_LEAST16_TYPE__	s16;
typedef __INT_LEAST32_TYPE__	s32;
typedef __INT_LEAST64_TYPE__	s64;

# if __x86_64__
typedef u64 size_t;
typedef s64 ssize_t;
#  define __SIZEOF_SIZE_T__ 8
#  define __SIZE_T_WIDTH__ __UINT_LEAST64_WIDTH__
#  define __SIZE_T_MAX__ __UINT_LEAST64_MAX__
# else
typedef u32 size_t;
typedef s32 ssize_t;
#  define __SIZEOF_SIZE_T__ 4
#  define __SIZE_T_WIDTH__ __UINT_LEAST32_WIDTH__
#  define __SIZE_T_MAX__ __UINT_LEAST32_MAX__
# endif

void		*memmove(void *, const void *, size_t);
void		*memcpy(void *, const void *, size_t);
void		*memchr(const void *, int, size_t);
int		memcmp(const void *, const void *, size_t);
void		*memset(void *, int, size_t);

size_t		strlen(const char *);
size_t		strnlen(const char *, size_t);
int		strcmp(const char *, const char *);
char		*strcpy(char *, const char *);
char		*strchr(const char *, int);

int		printk(const char *fmt, ...);

#endif
