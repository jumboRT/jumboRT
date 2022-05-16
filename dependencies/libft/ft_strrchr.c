/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   ft_strrchr.c                                     -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:58:39 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:58:39 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char
	*ft_strrchr(const char *str, int ch)
{
	char	*res;

	res = NULL;
	while (1)
	{
		if (*str == (char) ch)
			res = (char *) str;
		if (!*str)
			return (res);
		str += 1;
	}
}
