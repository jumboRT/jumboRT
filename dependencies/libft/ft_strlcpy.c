/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   ft_strlcpy.c                                     -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:58:36 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:58:36 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t
	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	len;

	len = ft_strlen(src);
	if (len + 1 < size)
		ft_memcpy(dst, src, len + 1);
	else if (size > 0)
	{
		ft_memcpy(dst, src, size - 1);
		dst[size - 1] = '\0';
	}
	return (len);
}
