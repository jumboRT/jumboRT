/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   random2.c                                      #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:16 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:16 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "rand.h"

t_vec
	rt_random_unit_sphere(GLOBAL t_seed *seed)
{
	float	a;
	float	b;
	float	aa;
	float	bb;

	while (1)
	{
		a = rt_random_gauss(seed);
		b = rt_random_gauss(seed);
		aa = a * a;
		bb = b * b;
		if (aa + bb >= 1.0f)
			continue ;
		return (vec3((2.0f * a) * rt_sqrt(1.0f - aa - bb),
				(2.0f * b) * rt_sqrt(1.0f - aa - bb),
				1.0f - (2.0f * rt_sqrt(aa + bb))));
	}
}

t_vec
	rt_random_on_hemi(GLOBAL t_seed *seed, t_vec normal)
{
	t_vec	result;

	result = vec_norm(rt_random_in_sphere(seed));
	if (vec_dot(result, normal) < 0)
		result = vec_neg(result);
	return (result);
}

/* https://stats.stackexchange.com/a/481544 */
t_vec
	rt_random_in_disk(GLOBAL t_seed *seed, t_vec right, t_vec up, float radius)
{
	float	r;
	float	alpha;
	float	x;
	float	y;

	r = rt_sqrt(rt_random_float_range(seed, 0.0, radius * radius));
	alpha = rt_random_float_range(seed, 0.0, RT_2PI);
	x = r * rt_cos(alpha);
	y = r * rt_sin(alpha);
	return (vec_add(vec_scale(right, x), vec_scale(up, y)));
}

t_vec2
	rt_random_concentric_disk(GLOBAL t_seed *seed)
{
	t_vec2	epsilon;
	float	theta;
	float	r;

	epsilon = vec2(rt_random_float(seed), rt_random_float(seed));
	epsilon = vec2_sub(vec2_scale(epsilon, 2.0f), vec2(1.0f, 1.0f));
	if (vec2_eq(epsilon, vec2_0()))
		return (epsilon);
	if (rt_abs(u(epsilon)) > rt_abs(v(epsilon)))
	{
		r = u(epsilon);
		theta = RT_PI_4 * (v(epsilon) / u(epsilon));
	}
	else
	{
		r = v(epsilon);
		theta = RT_PI_2 - RT_PI_4 * (u(epsilon) / v(epsilon));
	}
	return (vec2_scale(vec2(rt_cos(theta), rt_sin(theta)), r));
}

t_vec
	rt_random_cosine_hemi(GLOBAL t_seed *seed)
{
	t_vec2	d;
	float	z;

	d = rt_random_concentric_disk(seed);
	z = rt_sqrt(rt_max(0.0f, 1.0f - u(d) * u(d) - v(d) * v(d)));
	return (vec3(u(d), v(d), z));
}
