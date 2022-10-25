/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   vec_get.c                                      #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:22 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:22 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "rtmath.h"

#if !defined RT_VECTORIZE

float
	x(t_vec v)
{
	return (v.x);
}

float
	y(t_vec v)
{
	return (v.y);
}

float
	z(t_vec v)
{
	return (v.z);
}

float
	w(t_vec v)
{
	return (v.w);
}

float
	xyz(t_vec v, int i)
{
	if (i == 0)
		return (x(v));
	else if (i == 1)
		return (y(v));
	else
		return (z(v));
}

#endif
