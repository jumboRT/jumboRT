#include "rtmath.h"
#include "util.h"

#include <math.h>
#include <stddef.h>
#include <limits.h>

#include <stdio.h>
#include <stdlib.h>

t_seed
	rt_random(t_seed *seed)
{
	static t_seed	state = 7549087012;

	if (seed == NULL)
		seed = &state;
	*seed ^= *seed << 12;
	*seed ^= *seed >> 25;
	*seed ^= *seed << 27;
	return (*seed * 0x2545F4914F6CDD1DULL);
}

FLOAT
	rt_random_float(t_seed *seed)
{
	return (rt_random(seed) / (FLOAT) ULONG_MAX);
}

FLOAT
	rt_random_float_range(t_seed *seed, FLOAT min, FLOAT max)
{
	return (min + (max - min) * (rt_random_float(seed)));
}

void
	rt_random_range(t_seed *seed, size_t *dst, size_t n)
{
	size_t	i;
	size_t	j;
	size_t	tmp;

	i = 0;
	while (i < n)
	{
		dst[i] = i;
		i += 1;
	}
	i = 0;
	while (i < n - 1)
	{
		j = rt_random(seed) % (n - i - 1) + i + 1;
		tmp = dst[i];
		dst[i] = dst[j];
		dst[j] = tmp;
		i += 1;
	}
}

/* https://math.stackexchange.com/questions/1163260/random-directions-on-hemisphere-oriented-by-an-arbitrary-vector */
t_vec
    rt_random_hvec(t_seed *seed, t_vec dir)
{
	t_vec	vec;

	vec = rt_random_svec(seed);
	if (vec_dot(vec, dir) < 0.0)
		return (vec_neg(vec));
	return (vec);
}

t_vec
	rt_random_svec(t_seed *seed)
{
	t_vec	vec;

	while (1)
	{
		vec.v[X] = rt_random_float(seed) * 2 - 1;
		vec.v[Y] = rt_random_float(seed) * 2 - 1;
		vec.v[Z] = rt_random_float(seed) * 2 - 1;
		vec.v[W] = 0.0;
		if (vec_mag(vec) < 1.0)
			return (vec);
	}
}
