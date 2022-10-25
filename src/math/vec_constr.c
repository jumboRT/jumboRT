/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   vec_constr.c                                   #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:24 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:24 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "rtmath.h"

#if !defined RT_VECTORIZE

t_vec
	vec(float x, float y, float z, float w)
{
	t_vec	result;

	result.x = x;
	result.y = y;
	result.z = z;
	result.w = w;
	return (result);
}

t_vec
	vec_0(void)
{
	return (vec(0.0, 0.0, 0.0, 0.0));
}

t_vec
	vec_x(float x)
{
	return (vec(x, 0.0, 0.0, 0.0));
}

t_vec
	vec_y(float y)
{
	return (vec(0.0, y, 0.0, 0.0));
}

t_vec
	vec_z(float z)
{
	return (vec(0.0, 0.0, z, 0.0));
}
#endif
