/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   ft_strmapi.c                                     -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:58:37 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:58:37 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "../../include/memory.h"
#include <stdlib.h>

char
	*ft_strmapi(const char *str, char (*func)(unsigned int, char))
{
	char			*res;
	unsigned int	i;

	if (FT_SAFE >= 1 && str == NULL)
		return (NULL);
	res = sh_safe_malloc(ft_strlen(str) + 1);
	if (res == NULL)
		return (NULL);
	i = 0;
	while (str[i])
	{
		res[i] = func(i, str[i]);
		i += 1;
	}
	res[i] = '\0';
	return (res);
}
