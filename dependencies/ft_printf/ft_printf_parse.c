/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   ft_printf_parse.c                                -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:57:57 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:57:57 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <limits.h>

static void
	ft_printf_parse_flags(t_flags *flags, const char **fmt)
{
	flags->left = 0;
	flags->sign = 0;
	flags->space = 0;
	flags->alt = 0;
	flags->zero = 0;
	while (1)
	{
		if (**fmt == '-')
			flags->left = 1;
		else if (**fmt == '+')
			flags->sign = 1;
		else if (**fmt == ' ')
			flags->space = 1;
		else if (**fmt == '#')
			flags->alt = 1;
		else if (**fmt == '0')
			flags->zero = 1;
		else
			return ;
		*fmt += 1;
	}
}

static int
	ft_printf_parse_int(int *v, const char **fmt, va_list *args)
{
	if (**fmt == '*')
	{
		*fmt += 1;
		*v = va_arg(*args, int);
		return (0);
	}
	*v = 0;
	while (**fmt >= '0' && **fmt <= '9')
	{
		if (*v > INT_MAX / 10 || (*v * 10 > INT_MAX - (**fmt - '0')))
			return (-1);
		*v = *v * 10 + (**fmt - '0');
		*fmt += 1;
	}
	return (0);
}

int
	ft_printf_parse(t_flags *flags, const char **fmt, va_list *args)
{
	int	tmp;

	ft_printf_parse_flags(flags, fmt);
	if (ft_printf_parse_int(&tmp, fmt, args) < 0)
		return (-1);
	flags->width = tmp;
	if (tmp < 0)
	{
		flags->left = 1;
		flags->width = -tmp;
	}
	flags->precision = -1;
	if (**fmt == '.')
	{
		*fmt += 1;
		if (ft_printf_parse_int(&flags->precision, fmt, args) < 0)
			return (-1);
	}
	flags->format = **fmt;
	*fmt += 1;
	return (0);
}
