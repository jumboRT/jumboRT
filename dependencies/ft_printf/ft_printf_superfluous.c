/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   ft_printf_superfluous.c                          -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:57:59 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:57:59 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int
	ft_vprintf(const char *fmt, va_list *vlist)
{
	t_sink	sink;
	int		size;

	sink.type = sink_type_fd;
	sink.fd = 1;
	size = ft_printf_vxprintf(&sink, fmt, vlist);
	return (size);
}

int
	ft_fprintf(int fd, const char *fmt, ...)
{
	va_list	args;
	t_sink	sink;
	int		size;

	va_start(args, fmt);
	sink.type = sink_type_fd;
	sink.fd = fd;
	size = ft_printf_vxprintf(&sink, fmt, &args);
	va_end(args);
	return (size);
}

int
	ft_vfprintf(int fd, const char *fmt, va_list *vlist)
{
	t_sink	sink;
	int		size;

	sink.type = sink_type_fd;
	sink.fd = fd;
	size = ft_printf_vxprintf(&sink, fmt, vlist);
	return (size);
}

int
	ft_sprintf(char *buffer, const char *fmt, ...)
{
	va_list	args;
	t_sink	sink;
	int		size;

	va_start(args, fmt);
	sink.type = sink_type_str;
	sink.str = buffer;
	*sink.str = '\0';
	size = ft_printf_vxprintf(&sink, fmt, &args);
	va_end(args);
	return (size);
}

int
	ft_vsprintf(char *buffer, const char *fmt, va_list *vlist)
{
	t_sink	sink;
	int		size;

	sink.type = sink_type_str;
	sink.str = buffer;
	*sink.str = '\0';
	size = ft_printf_vxprintf(&sink, fmt, vlist);
	return (size);
}
