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
#include "../includes/terminal.h"
#include <stdarg.h>

#define FMT_TYPE_ERROR		-1

#define FMT_TYPE_USIGNED	0
#define FMT_TYPE_OCTAL		FMT_TYPE_USIGNED
#define FMT_TYPE_HEXA		FMT_TYPE_USIGNED

#define FMT_TYPE_SIGNED		1

#define FMT_TYPE_CHAR		2
#define FMT_TYPE_STRING		3
#define FMT_TYPE_PTR		4

#define FMT_UPPER_HEX "0123456789ABCDEF"
#define FMT_LOWER_HEX "0123456789abcdef"
#define FMT_HEX FMT_UPPER_HEX FMT_LOWER_HEX

#define FMT_FLAG_MINUS 	0x01
#define FMT_FLAG_SPACE 	0x02
#define FMT_FLAG_SHARP  0x04
#define FMT_FLAG_PLUS  	0x08
#define FMT_FLAG_ZERO  	0x10
#define FMT_FLAG_PREC  	0x20
#define FMT_FLAG_SMALL  0x40

#define FMT_FLAG_LONG			0x100
#define FMT_FLAG_XLONG       	0x200
#define FMT_FLAG_SHORT      	0x400
#define FMT_FLAG_XSHORT			0x800

#define FMT_TYPE_ULONG FMT_FLAG_LONG
#define FMT_TYPE_UXLONG FMT_FLAG_XLONG
#define FMT_TYPE_USHORT FMT_FLAG_SHORT
#define FMT_TYPE_UXSHORT FMT_FLAG_XSHORT
#define FMT_TYPE_SLONG (FMT_FLAG_LONG | FMT_TYPE_SIGNED)
#define FMT_TYPE_SXLONG (FMT_FLAG_XLONG | FMT_TYPE_SIGNED)
#define FMT_TYPE_SSHORT (FMT_FLAG_SHORT | FMT_TYPE_SIGNED)
#define FMT_TYPE_SXSHORT (FMT_FLAG_XSHORT | FMT_TYPE_SIGNED)

#define PTR_FLAG_DUMP 0x1

#define zero_flag(s) ((s) & FMT_FLAG_ZERO)
#define space_flag(s) ((s) & FMT_FLAG_SPACE)
#define minus_flag(s) ((s) & FMT_FLAG_MINUS)
#define plus_flag(s) ((s) & FMT_FLAG_PLUS)
#define prec_flag(s) ((s) & FMT_FLAG_PREC)
#define sharp_flag(s) ((s) & FMT_FLAG_SHARP)
#define small_flag(s) ((s) & FMT_FLAG_SMALL)

#define PRINTK_BUFSIZE 1024

static char printk_buf[PRINTK_BUFSIZE];
static size_t used;

struct fmt_spec
{
	va_list args;

	int prec;
	int width;

	int flag;
	int type;
	int base;

	int __ptrflag;
	int __dumpsize;
	u32 val;
};

int	vprintk(const char *fmt, va_list args);

int	printk(const char *fmt, ...)
{
	int ret = 0;
	va_list args;

	va_start(args, fmt);
	ret = vprintk(fmt, args);
	va_end(args);
	return ret;
}

static int __adv_atoi(const char **fmt)
{
	register int r = 0;
	const char *cfmt = *fmt;

	for (; is_digit(*cfmt); ++cfmt) {
		r *= 10;
		r += *cfmt - 0x30;
	}
	*fmt = cfmt;
	return r;
}

static int _fmt_decode(const char *fmt, struct fmt_spec *opt)
{
	const char *old_fmt = fmt;
	int length;

	opt->flag = 0;
	opt->type = 0;
	opt->prec = 0;
	opt->width = 0;
	opt->__ptrflag = 0;

	for (;;) {
		switch (*++fmt) {
		case '-': opt->flag |= FMT_FLAG_MINUS; break;
		case '+': opt->flag |= FMT_FLAG_PLUS;  break;
		case ' ': opt->flag |= FMT_FLAG_SPACE; break;
		case '0': opt->flag |= FMT_FLAG_ZERO;  break;
		default: goto field_width;
		}
	}

field_width:
	if ('*' == *fmt) {
		++fmt;
		opt->width = va_arg(opt->args, int);
		if (0 > opt->width) {
			opt->flag |= FMT_FLAG_MINUS;
			opt->width = (~(opt->width) & __INT_LEAST16_MAX__) + 1;
		}
		goto precision;
	}
	if (is_digit(*fmt))
		opt->width = __adv_atoi(&fmt);
precision:
	if ('.' != *fmt)
		goto qualifier;
	++fmt;
	opt->flag |= FMT_FLAG_PREC;
	if ('*' == *fmt) {
		++fmt;
		opt->prec = va_arg(opt->args, int) & __INT_LEAST16_MAX__;
		goto qualifier;
	}
	opt->prec = __adv_atoi(&fmt);

qualifier:
	length = *fmt;
	if (length == 'l' || length == 'h') {
		if (length != *++fmt)
			goto type;
		length += 0x20;
		++fmt;
	}

type:
	opt->base = 10;
	switch (*fmt++) {
	case 'u': opt->type |= FMT_TYPE_USIGNED;break;
	case 'd': opt->type |= FMT_TYPE_SIGNED; break;
	case 'o': opt->base = 8;		break;
	case 'x': opt->flag |= FMT_FLAG_SMALL; __attribute__ ((fallthrough));
	case 'X': opt->base = 16;		break;
	case 'c': opt->type |= FMT_TYPE_CHAR;	goto getval;
	case 's': opt->type |= FMT_TYPE_STRING; goto getval;
	case 'p': opt->type |= FMT_TYPE_PTR;	goto fmt_ptr;
	default:  opt->type = FMT_TYPE_ERROR;   goto end;
	}

	switch (length) {
	case 'L': opt->type |= FMT_FLAG_XLONG;    goto getval;
	case 'l': opt->type |= FMT_FLAG_LONG;     goto getval;
	case 'h': opt->type |= FMT_FLAG_SHORT;    goto getval;
	case 'H': opt->type |= FMT_FLAG_XSHORT;   goto getval;
	}
fmt_ptr:

getval:
	switch (opt->type) {
	case FMT_TYPE_STRING:
	case FMT_TYPE_PTR:
	case FMT_TYPE_ULONG:
	case FMT_TYPE_UXLONG:
	case FMT_TYPE_USIGNED:
		opt->val = va_arg(opt->args, u32);
		break ;
	case FMT_TYPE_USHORT:
		opt->val = (u16)va_arg(opt->args, int);
		break ;
	case FMT_TYPE_UXSHORT:
		opt->val = (u8)va_arg(opt->args, int);
		break ;
	case FMT_TYPE_SLONG:
	case FMT_TYPE_SXLONG:
	case FMT_TYPE_SIGNED:
		opt->val = va_arg(opt->args, s32);
		break ;
	case FMT_TYPE_SSHORT:
		opt->val = (s16)va_arg(opt->args, int);
		break ;
	case FMT_TYPE_CHAR:
	case FMT_TYPE_SXSHORT:
		opt->val = (s8)va_arg(opt->args, int);
	}
end:
	return fmt - old_fmt;
}

static void _put_flush()
{
	terminal_write(printk_buf, used);
	used = 0;
}

static int _put_repeat(int c, size_t times)
{
	size_t remainder = times;

	if (times + used > PRINTK_BUFSIZE) {
		_put_flush();
		if (times >= PRINTK_BUFSIZE)
			memset(printk_buf, c, PRINTK_BUFSIZE);
		for (; remainder > PRINTK_BUFSIZE;
		     remainder -= PRINTK_BUFSIZE)
			terminal_write(printk_buf, PRINTK_BUFSIZE);
	}
	memset(printk_buf + used, c, remainder);
	used += remainder;
	return times;
}

static int _put_buffer(const void *s, size_t size)
{
	size_t remainder = size;
	size_t offset = 0;

	if (size + used > PRINTK_BUFSIZE) {
		_put_flush();
		for (; remainder > PRINTK_BUFSIZE;
		     remainder -= PRINTK_BUFSIZE, offset += PRINTK_BUFSIZE)
			terminal_write(s + offset, PRINTK_BUFSIZE);
	}
	memcpy(printk_buf + used, s + offset, remainder);
	used += size;
	return size;
}

static int _print_numbers(struct fmt_spec *opt)
{
	static char _itoabuf[64];
	char *buf = _itoabuf + 64;
	const char *hexbase = FMT_HEX;
	size_t bufsize = 0, wrote = 0;
	register u32 val = opt->val, base = opt->base;
	int neg = 0, shift;
	char sign = 0;

	if (opt->type & FMT_TYPE_SIGNED) {
		neg = opt->val > __INT_LEAST32_MAX__;
		if (neg) {
			val = -val;
			sign = '-';
			opt->width--;
		} else if (plus_flag(opt->flag)) {
			sign = '+';
			opt->width--;
		} else if (space_flag(opt->flag)) {
			sign = ' ';
			opt->width--;
		}
	}

	if (base == 10) {
		for (; val; val /= base)
			*--buf = (val % base) + 0x30;
	} else {
		shift = 3;
		if (base == 16)
			shift = 4;
		if (small_flag(opt->flag))
			hexbase += 16;
		for (; val; val >>= shift)
			*--buf = hexbase[val & (base - 1)];
	}

	bufsize = (_itoabuf + 64) - buf;
	if (bufsize == 0) {
		*--buf = '0';
		bufsize = 1;
	}

	if (minus_flag(opt->flag))
		opt->flag &= ~FMT_FLAG_ZERO;

	if (prec_flag(opt->flag)) {
		opt->flag &= ~FMT_FLAG_ZERO;
		if (bufsize < (size_t)opt->prec)
			opt->prec = bufsize;
	}
	opt->width -= opt->prec;
	opt->prec -= bufsize;

	if (!(minus_flag(opt->flag) && zero_flag(opt->flag)) &&	\
	    0 < opt->width)
		wrote += _put_repeat(' ', opt->width);

	if (sign)
		wrote += _put_repeat(sign, 1);

	if (zero_flag(opt->flag) &&		\
	    0 < opt->width)
		wrote += _put_repeat('0', opt->width);

	if (prec_flag(opt->flag) &&		\
	    0 < opt->prec)
		wrote += _put_repeat('0', opt->prec);

	wrote += _put_buffer(buf, bufsize);

	if (minus_flag(opt->flag) &&		\
	    0 < opt->width)
		wrote += _put_repeat(' ', opt->width);
	return wrote;
}

static int _print_char(struct fmt_spec *opt)
{
	const char c = (char)opt->val;
	size_t wrote = 0;

	opt->width -= 1;
	if (!minus_flag(opt->flag) &&		\
	    0 < opt->width)
		wrote += _put_repeat(' ', opt->width);

	wrote += _put_repeat(c, 1);

	if (minus_flag(opt->flag) &&		\
	    0 < opt->width)
		wrote += _put_repeat(' ', opt->width);
	return wrote;
}

static int _print_string(struct fmt_spec *opt)
{
	const char *buf = (char *)opt->val;
	size_t wrote = 0;
	size_t buflen = prec_flag(opt->flag) ?			\
		strnlen(buf, opt->prec) :			\
		strnlen(buf, __SIZE_T_MAX__ - opt->val);

	opt->width -= buflen;
	if (!minus_flag(opt->flag) &&		\
	    0 < opt->width)
		wrote += _put_repeat(' ', opt->width);

	wrote += _put_buffer(buf, buflen);

	if (minus_flag(opt->flag) &&		\
	    0 < opt->width)
		wrote += _put_repeat(' ', opt->width);
	return wrote;
}

static int _print_ptr(struct fmt_spec *opt)
{
	// TODO
	(void)opt;
	return 0;
}

int		vprintk(const char *fmt, va_list args)
{
	const char *cfmt;
	struct fmt_spec opt;
	int ret = 0;
	int wrote = 0;

	opt.args = args;
	for (cfmt = strchr(fmt, '%');					\
	     0 != cfmt;							\
	     cfmt = strchr(cfmt, '%')) {
		wrote += _put_buffer(fmt, cfmt - fmt);

		ret = _fmt_decode(cfmt, &opt);

		cfmt += ret;
		fmt = cfmt;

		switch (opt.type) {
		case FMT_TYPE_ERROR:
			wrote += _put_buffer(cfmt - ret, ret);
			break ;
		case FMT_TYPE_CHAR:
			wrote += _print_char(&opt);
			break ;
		case FMT_TYPE_STRING:
			wrote += _print_string(&opt);
			break ;
		case FMT_TYPE_PTR:
			wrote += _print_ptr(&opt);
			break ;
		default:
			wrote += _print_numbers(&opt);
		}
	}
	wrote += _put_buffer(fmt, strlen(fmt));
	_put_flush();
	return wrote;
}
