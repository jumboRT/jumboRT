/* ************************************************************************** */
/*                                                                            */
/*                                                           :      .         */
/*   shft_atol.c                                      -=-:::+*+:-+*#.         */
/*                                                :-:::+#***********----:     */
/*   By: csteenvo <csteenvo@student.codam.n>        .:-*#************#-       */
/*                                                 :=+*+=+*********####+:     */
/*   Created: 2022/05/10 11:58:41 by csteenvo     ..     +**=-=***-           */
/*   Updated: 2022/05/10 11:58:41 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <limits.h>

long
	ft_atol(const char *str)
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
			return (LONG_MAX);
		if (v < LONG_MIN / 10 || (s < 0 && v * 10 < LONG_MIN + (*str - '0')))
			return (LONG_MIN);
		v = v * 10 + (*str - '0') * s;
		str += 1;
	}
	return (v);
}
