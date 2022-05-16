/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   ft_strchr.c                                      -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:58:33 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:58:33 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char
	*ft_strchr(const char *str, int ch)
{
	while (1)
	{
		if (*str == (char) ch)
			return ((char *) str);
		if (!*str)
			return (NULL);
		str += 1;
	}
}
