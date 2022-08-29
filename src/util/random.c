#include "cl.h"

t_seed
	rt_random(GLOBAL t_seed *seed)
{
	*seed ^= *seed << 12;
	*seed ^= *seed >> 25;
	*seed ^= *seed << 27;
	return (*seed * (((t_seed) 0x2545F491 << 32) | 0x4F6CDD1D));
}

float
	rt_random_float(GLOBAL t_seed *seed)
{
	return (rt_random(seed) / 18446744073709551616.0);
}

float
	rt_random_float_range(GLOBAL t_seed *seed, float min, float max)
{
	return (min + (max - min) * (rt_random_float(seed)));
}

t_vec
	rt_random_in_sphere(GLOBAL t_seed *seed)
{
	t_vec	result;
	float	len;

	while (1)
	{
		result = vec(
				rt_random_float_range(seed, -1, 1),
				rt_random_float_range(seed, -1, 1),
				rt_random_float_range(seed, -1, 1),
				0.0);
		len = vec_mag(result);
		if (len <= 1)
			return (result);
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
