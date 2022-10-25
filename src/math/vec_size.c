/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   vec_size.c                                     #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:24 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:24 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "rtmath.h"

#if !defined RT_VECTORIZE

float
	vec_mag2(t_vec a)
{
	return (vec_dot(a, a));
}

float
	vec_mag(t_vec a)
{
	return (rt_sqrt(vec_mag2(a)));
}

t_vec
	vec_norm(t_vec v)
{
	return (vec_scale(v, (float) 1.0 / vec_mag(v)));
}

/* Safe version of vec_norm that doesn't divide by 0 */
t_vec
	vec_norm2(t_vec v)
{
	float	len;

	len = vec_mag(v);
	if (len == 0)
		return (vec_z(1.0));
	return (vec_scale(v, (float) 1.0 / len));
}

#endif
