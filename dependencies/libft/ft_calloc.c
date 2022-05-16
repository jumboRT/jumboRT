/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   ft_calloc.c                                      -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:58:17 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:58:17 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "../../include/memory.h"
#include <stdlib.h>

void
	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;

	ptr = sh_safe_malloc(count * size);
	if (ptr == NULL)
		return (NULL);
	ft_bzero(ptr, count * size);
	return (ptr);
}
