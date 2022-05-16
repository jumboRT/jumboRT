/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   ft_printf.c                                      -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:57:39 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:57:44 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int
	ft_printf_print(t_sink *sink, t_flags *flags, va_list *args)
{
	if (flags->format == 'c')
		return (ft_printf_chr(sink, flags, va_arg(*args, int)));
	if (flags->format == 's')
		return (ft_printf_str(sink, flags, va_arg(*args, const char *)));
	if (flags->format == '%')
		return (ft_printf_chr(sink, flags, '%'));
	if (flags->format == 'd' || flags->format == 'i')
		return (ft_printf_int(sink, flags, va_arg(*args, int)));
	if (flags->format == 'u' || flags->format == 'x' || flags->format == 'X')
		return (ft_printf_pos(sink, flags, va_arg(*args, unsigned int)));
	if (flags->format == 'p')
		return (ft_printf_pos(sink, flags, (size_t) va_arg(*args, void *)));
	return (-1);
}

int
	ft_printf_vxprintf(t_sink *sink, const char *fmt, va_list *args)
{
	t_flags	flags;
	int		size;
	int		tmp;

	size = 0;
	while (1)
	{
		tmp = 0;
		while (fmt[tmp] && fmt[tmp] != '%')
			tmp += 1;
		if (ft_printf_write(sink, fmt, tmp) < 0)
			return (-1);
		size += tmp;
		if (!fmt[tmp])
			return (size);
		fmt += tmp + 1;
		if (ft_printf_parse(&flags, &fmt, args) < 0)
			return (-1);
		tmp = ft_printf_print(sink, &flags, args);
		if (tmp < 0)
			return (-1);
		size += tmp;
	}
}

int
	ft_printf(const char *fmt, ...)
{
	va_list	args;
	t_sink	sink;
	int		size;

	va_start(args, fmt);
	sink.type = sink_type_fd;
	sink.fd = 1;
	size = ft_printf_vxprintf(&sink, fmt, &args);
	va_end(args);
	return (size);
}
