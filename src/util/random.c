#include "rtmath.h"
#include "util.h"

#include <math.h>
#include <stddef.h>
#include <limits.h>

/* TODO: not thread safe but used from multiple threads */
unsigned long
	rt_random(void)
{
	static unsigned long	state = 7549087012;

	state ^= state << 12;
	state ^= state >> 25;
	state ^= state << 27;
	return (state * 0x2545F4914F6CDD1DULL);
}

FLOAT
	rt_random_float(void)
{
	return (rt_random() / (FLOAT) ULONG_MAX);
}

FLOAT
	rt_random_float_range(FLOAT min, FLOAT max)
{
	return (min + (max - min) * (rt_random_float()));
}

void
	rt_random_range(size_t *dst, size_t n)
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
		j = rt_random() % (n - i - 1) + i + 1;
		tmp = dst[i];
		dst[i] = dst[j];
		dst[j] = tmp;
		i += 1;
	}
}

t_vec
	rt_random_unit_vector(t_vec normal)
{
	t_vec	vec;

	vec.v[W] = 0.0;
	vec.v[X] = rt_random_float();
	vec.v[Y] = rt_random_float();
	vec.v[Z] = rt_random_float();
	return (vec);
}
