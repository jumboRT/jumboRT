/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   vec_clamp.c                                    #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:22 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:22 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "rtmath.h"

#if !defined RT_VECTORIZE

t_vec
	vec_min(t_vec a, t_vec b)
{
	a.x = rt_min(a.x, b.x);
	a.y = rt_min(a.y, b.y);
	a.z = rt_min(a.z, b.z);
	a.w = rt_min(a.w, b.w);
	return (a);
}

t_vec
	vec_max(t_vec a, t_vec b)
{
	a.x = rt_max(a.x, b.x);
	a.y = rt_max(a.y, b.y);
	a.z = rt_max(a.z, b.z);
	a.w = rt_max(a.w, b.w);
	return (a);
}

t_vec
	vec_clamp(t_vec v, float min, float max)
{
	v.x = rt_max(rt_min(v.x, max), min);
	v.y = rt_max(rt_min(v.y, max), min);
	v.z = rt_max(rt_min(v.z, max), min);
	v.w = rt_max(rt_min(v.w, max), min);
	return (v);
}

#endif
