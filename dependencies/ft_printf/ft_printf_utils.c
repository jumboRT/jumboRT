/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   ft_printf_utils.c                                -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:58:00 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:58:00 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <unistd.h>

void	*rt_realloc(void *ptr, size_t old_size, size_t new_size);
void	*ft_memcpy(void *dst, const void *src, size_t n);

int
	ft_printf_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i += 1;
	return (i);
}

void
	ft_printf_reserve(t_sink *sink, size_t size)
{
	if (size > sink->capacity)
	{
		if (size > sink->size * 2)
			sink->capacity = size;
		else
			sink->capacity = sink->size * 2;
		sink->str = rt_realloc(sink->str, sink->size + 1, sink->capacity + 1);
	}
}

int
	ft_printf_write(t_sink *sink, const char *str, int size)
{
	if (sink->type == sink_type_fd)
		return (write(sink->fd, str, size));
	else if (sink->type == sink_type_str)
	{
		if (sink->str != NULL)
		{
			ft_memcpy(sink->str, str, size);
			sink->str += size;
			*sink->str = '\0';
		}
		return (size);
	}
	else if (sink->type == sink_type_astr)
	{
		ft_printf_reserve(sink, sink->size + size);
		ft_memcpy(sink->str + sink->size, str, size);
		sink->size += size;
		sink->str[sink->size] = '\0';
		return (size);
	}
	return (-1);
}

int
	ft_printf_repeat(t_sink *sink, char ch, int size)
{
	char	buf[256];
	int		i;

	i = 0;
	while (i < 256)
	{
		buf[i] = ch;
		i += 1;
	}
	i = 0;
	while (size - i > 256)
	{
		if (ft_printf_write(sink, buf, 256) < 0)
			return (-1);
		i += 256;
	}
	if (i < size)
	{
		if (ft_printf_write(sink, buf, size - i) < 0)
			return (-1);
		i += size - i;
	}
	return (i);
}
