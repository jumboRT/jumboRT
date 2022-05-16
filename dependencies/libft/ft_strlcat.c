/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   ft_strlcat.c                                     -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:58:35 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:58:35 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t
	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	len;

	len = 0;
	while (len < size && dst[len])
		len += 1;
	return (len + ft_strlcpy(dst + len, src, size - len));
}
