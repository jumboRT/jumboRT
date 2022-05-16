/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   ft_strncmp.c                                     -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:58:38 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:58:38 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int
	ft_strncmp(const char *str1, const char *str2, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		if ((unsigned char) str1[i] != (unsigned char) str2[i])
			return ((unsigned char) str1[i] - (unsigned char) str2[i]);
		if ((unsigned char) str1[i] == 0)
			return (0);
		i += 1;
	}
	return (0);
}
