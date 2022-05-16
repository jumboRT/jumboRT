/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   ft_printf_integer.c                              -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:57:54 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:57:55 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static void
	ft_printf_atoi(char **buf, size_t value, const char *charset, size_t base)
{
	*buf -= 1;
	**buf = '\0';
	while (value > 0)
	{
		*buf -= 1;
		**buf = charset[value % base];
		value /= base;
	}
}

static int
	ft_printf_pad_init(int precision, int *size, int a_size, int b_size)
{
	if (precision < 0)
		precision = 1;
	if (precision > a_size)
		*size = precision + b_size;
	else
		*size = a_size + b_size;
	return (precision);
}

static int
	ft_printf_pad(t_sink *sink, t_flags *flags, const char *a, const char *b)
{
	int	precision;
	int	a_size;
	int	b_size;
	int	size;
	int	tmp;

	a_size = ft_printf_strlen(a);
	b_size = ft_printf_strlen(b);
	precision = ft_printf_pad_init(flags->precision, &size, a_size, b_size);
	tmp = " 0"[flags->zero && flags->precision < 0];
	if (tmp == '0' && ft_printf_write(sink, b, b_size) < 0)
		return (-1);
	if (!flags->left && ft_printf_repeat(sink, tmp, flags->width - size) < 0)
		return (-1);
	if (tmp == ' ' && ft_printf_write(sink, b, b_size) < 0)
		return (-1);
	if (ft_printf_repeat(sink, '0', precision - a_size) < 0)
		return (-1);
	if (ft_printf_write(sink, a, a_size) < 0)
		return (-1);
	if (flags->left && ft_printf_repeat(sink, ' ', flags->width - size) < 0)
		return (-1);
	if (flags->width > (unsigned int) size)
		return (flags->width);
	return (size);
}

int
	ft_printf_int(t_sink *sink, t_flags *flags, int value)
{
	char	buf[256];
	char	*ptr;

	ptr = buf + 256;
	if (value < 0)
		ft_printf_atoi(&ptr, (unsigned int) -value, "0123456789", 10);
	if (value >= 0)
		ft_printf_atoi(&ptr, value, "0123456789", 10);
	if (flags->sign && value >= 0)
		return (ft_printf_pad(sink, flags, ptr, "+"));
	if (flags->space && value >= 0)
		return (ft_printf_pad(sink, flags, ptr, " "));
	if (value < 0)
		return (ft_printf_pad(sink, flags, ptr, "-"));
	return (ft_printf_pad(sink, flags, ptr, ""));
}

int
	ft_printf_pos(t_sink *sink, t_flags *flags, size_t value)
{
	char		buf[256];
	char		*ptr;

	ptr = buf + 256;
	if (flags->format == 'u')
		ft_printf_atoi(&ptr, value, "0123456789", 10);
	if (flags->format == 'x' || flags->format == 'p')
		ft_printf_atoi(&ptr, value, "0123456789abcdef", 16);
	if (flags->format == 'X')
		ft_printf_atoi(&ptr, value, "0123456789ABCDEF", 16);
	if (flags->format == 'x' && flags->alt && value != 0)
		return (ft_printf_pad(sink, flags, ptr, "0x"));
	if (flags->format == 'X' && flags->alt && value != 0)
		return (ft_printf_pad(sink, flags, ptr, "0X"));
	if (flags->format == 'p')
		return (ft_printf_pad(sink, flags, ptr, "0x"));
	return (ft_printf_pad(sink, flags, ptr, ""));
}
