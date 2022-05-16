/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   ft_memchr.c                                      -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:58:28 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:58:28 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void
	*ft_memchr(const void *ptr, int ch, size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
	{
		if (((unsigned char *) ptr)[i] == (unsigned char) ch)
			return ((unsigned char *) ptr + i);
		i += 1;
	}
	return (NULL);
}
