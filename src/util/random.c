#include "rand.h"

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
	rt_random_gauss(GLOBAL t_seed *seed)
{
	float	sum;
	int	idx;

	sum = 0.0f;
	idx = 0;
	while (idx < 12)
	{
		sum += rt_random_float(seed);
		idx++;
	}
	return (6.0f - sum);
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
		result = vec(rt_random_float_range(seed, -1, 1), rt_random_float_range(seed, -1, 1), rt_random_float_range(seed, -1, 1), 0.0);
		len = vec_mag(result);
		if (len <= 1)
			return (result);
	}
}

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
			continue;
		return vec3((2.0f * a) * rt_sqrt(1.0f - aa - bb),
				(2.0f * b) * rt_sqrt(1.0f - aa - bb),
				1.0f - (2.0f * rt_sqrt(aa + bb)));
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

t_vec
	rt_random_on_hemi_cos(GLOBAL t_seed *seed)
{
	float	a;
	float	b;
	float	u1;
	float	u2;

	u1 = rt_random_float(seed);
	u2 = rt_random_float(seed);
	a = rt_sqrt(u1);
	b = RT_2PI * u2;
	return (vec3(a * rt_cos(b),
				a * rt_sin(b),
				rt_sqrt(1.0f - u1)));
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
