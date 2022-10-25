/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   aabb.c                                         #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:17 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:17 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "aabb.h"
#include "rtmath.h"

t_bounds
	bounds(t_vec min, t_vec max)
{
	t_bounds	bounds;

	bounds.min = min;
	bounds.max = max;
	return (bounds);
}

t_bounds
	bounds_0(void)
{
	t_vec	min;

	min = vec(RT_HUGE_VAL, RT_HUGE_VAL, RT_HUGE_VAL, 0.0);
	return (bounds(min, vec_neg(min)));
}

t_bounds
	bounds_max(t_bounds a, t_bounds b)
{
	return (bounds(vec_min(a.min, b.min), vec_max(a.max, b.max)));
}

float
	bounds_surf(t_bounds a)
{
	t_vec	s;

	s = vec_sub(a.max, a.min);
	return (rt_abs(x(s) * y(s) * 2)
		+ rt_abs(x(s) * z(s) * 2)
		+ rt_abs(y(s) * z(s) * 2));
}
