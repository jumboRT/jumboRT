/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   ft_split.c                                       -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:58:33 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:58:33 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "../../include/memory.h"
#include <stdlib.h>

static size_t
	split(const char *str, char ch, char **dst)
{
	const char	*start;
	size_t		count;

	count = 0;
	start = str;
	while (1)
	{
		if (*str == ch || !*str)
		{
			if (str != start)
			{
				if (dst != NULL)
				{
					dst[count] = ft_substr(start, 0, str - start);
					if (dst[count] == NULL)
						return (count);
				}
				count += 1;
			}
			if (!*str)
				return (count);
			start = str + 1;
		}
		str += 1;
	}
}

char
	**ft_split(const char *str, char ch)
{
	char	**res;
	size_t	count1;
	size_t	count2;

	if (FT_SAFE >= 1 && str == NULL)
		return (NULL);
	count1 = split(str, ch, NULL);
	res = sh_safe_malloc(sizeof(*res) * (count1 + 1));
	if (res == NULL)
		return (NULL);
	count2 = split(str, ch, res);
	if (count1 != count2)
	{
		while (0 < count2)
		{
			count2 -= 1;
			free(res[count2]);
		}
		free(res);
		return (NULL);
	}
	res[count1] = NULL;
	return (res);
}
