/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   shft_strndup.c                                   -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:58:52 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:58:52 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

#include "../../include/memory.h"

char
	*ft_strndup(const char *s1, size_t n)
{
	size_t		len;
	char		*dup;
	const char	*cpy;

	len = 0;
	cpy = s1;
	while (n && *cpy)
	{
		len++;
		cpy++;
		n--;
	}
	dup = sh_safe_malloc(len + 1);
	if (dup == NULL)
		return (NULL);
	ft_memcpy(dup, s1, len);
	dup[len] = '\0';
	return (dup);
}
