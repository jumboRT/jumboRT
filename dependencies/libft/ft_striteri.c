/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   ft_striteri.c                                    -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:58:34 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:58:34 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void
	ft_striteri(char *str, void (*func)(unsigned int, char*))
{
	unsigned int	i;

	if (FT_SAFE >= 1 && str == NULL)
		return ;
	i = 0;
	while (str[i])
	{
		func(i, str + i);
		i += 1;
	}
}
