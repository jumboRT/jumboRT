/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   ft_printf_string.c                               -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:57:59 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:57:59 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int
	ft_printf_pad(t_sink *sink, t_flags *flags, const char *value, int size)
{
	int	tmp;

	tmp = " 0"[flags->zero];
	if (!flags->left && ft_printf_repeat(sink, tmp, flags->width - size) < 0)
		return (-1);
	if (ft_printf_write(sink, value, size) < 0)
		return (-1);
	if (flags->left && ft_printf_repeat(sink, ' ', flags->width - size) < 0)
		return (-1);
	if (flags->width > (unsigned int) size)
		return (flags->width);
	return (size);
}

int
	ft_printf_chr(t_sink *sink, t_flags *flags, int value)
{
	char	ch;

	ch = value;
	return (ft_printf_pad(sink, flags, &ch, 1));
}

int
	ft_printf_str(t_sink *sink, t_flags *flags, const char *value)
{
	int	size;

	if (value == NULL)
		value = "(null)";
	size = ft_printf_strlen(value);
	if (flags->precision >= 0 && flags->precision < size)
		size = flags->precision;
	return (ft_printf_pad(sink, flags, value, size));
}
