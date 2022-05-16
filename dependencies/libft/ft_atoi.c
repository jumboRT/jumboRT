/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   ft_atoi.c                                        -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:58:11 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:58:13 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <limits.h>

int
	ft_atoi(const char *str)
{
	long	v;
	int		s;

	while ((*str >= 9 && *str <= 13) || *str == 32)
		str += 1;
	s = 1;
	if (*str == '-')
		s = -s;
	if (*str == '-' || *str == '+')
		str += 1;
	v = 0;
	while (ft_isdigit(*str))
	{
		if (v > LONG_MAX / 10 || (s > 0 && v * 10 > LONG_MAX - (*str - '0')))
			return ((int)(LONG_MAX));
		if (v < LONG_MIN / 10 || (s < 0 && v * 10 < LONG_MIN + (*str - '0')))
			return ((int)(LONG_MIN));
		v = v * 10 + (*str - '0') * s;
		str += 1;
	}
	return (v);
}
