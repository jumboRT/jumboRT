/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   vec_rotate.c                                   #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:24 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:24 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "rtmath.h"

t_vec
	vec_rotate(t_vec axis, t_vec v, float angle)
{
	return (vec_add(
			vec_add(
				vec_scale(v, rt_cos(angle)),
				vec_scale(vec_cross(axis, v), rt_sin(angle))),
			vec_scale(axis,
				vec_dot(axis, v) * (1.0 - rt_cos(angle)))));
}

void
	vec_angles(t_vec basis, t_vec v, t_vec *out_vec, float *out_angle)
{
	*out_vec = vec_norm2(vec_cross(basis, v));
	*out_angle = rt_acos(rt_min(1, rt_max(-1, vec_dot(basis, v))));
}
