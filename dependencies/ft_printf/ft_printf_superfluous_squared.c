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
	ft_asprintf(char **buffer, const char *fmt, ...)
{
	va_list	args;
	t_sink	sink;
	int		size;

	va_start(args, fmt);
	sink.type = sink_type_astr;
	sink.str = NULL;
	size = ft_printf_vxprintf(&sink, fmt, &args);
	va_end(args);
	*buffer = sink.str;
	return (size);
}

int
	ft_vasprintf(char **buffer, const char *fmt, va_list *vlist)
{
	t_sink	sink;
	int		size;

	sink.type = sink_type_astr;
	sink.str = NULL;
	size = ft_printf_vxprintf(&sink, fmt, vlist);
	*buffer = sink.str;
	return (size);
}
