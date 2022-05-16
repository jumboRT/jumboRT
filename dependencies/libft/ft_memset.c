/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   ft_memset.c                                      -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:58:30 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:58:30 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void
	*ft_memset(void *ptr, int ch, size_t size)
{
	while (size)
	{
		size -= 1;
		((unsigned char *) ptr)[size] = ch;
	}
	return (ptr);
}
