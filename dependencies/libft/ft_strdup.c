/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   ft_strdup.c                                      -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:58:34 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:58:34 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "../../include/util.h"
#include <stdlib.h>

char
	*ft_strdup(const char *str)
{
	size_t	len;
	char	*res;

	len = ft_strlen(str);
	res = rt_malloc(len + 1);
	if (res == NULL)
		return (NULL);
	ft_memcpy(res, str, len + 1);
	return (res);
}
