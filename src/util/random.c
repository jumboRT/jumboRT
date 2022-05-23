#include "rtmath.h"
#include "util.h"

#include <math.h>
#include <stddef.h>
#include <limits.h>


#include <stdio.h>

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

/* https://math.stackexchange.com/questions/1163260/random-directions-on-hemisphere-oriented-by-an-arbitrary-vector */
t_vec
    rt_random_hvec(t_vec dir)
{
    FLOAT   theta0;
    FLOAT   theta1;
    t_vec   out;
    t_vec   bef;
    t_vec   axis;

    theta0 = RT_2PI * rt_random_float();
    theta1 = acos(1 - 2 * rt_random_float());
    out.v[X] = sin(theta0) * sin(theta1);
    out.v[Y] = sin(theta0) * cos(theta1);
    out.v[Z] = fabs(sin(theta1));
    out.v[W] = 0.0;
    axis = vec_cross(dir, vec(0.0, 0.0, 1.0, 0.0));
    bef = out;
    out = vec_rotate(axis, out, acos(dir.v[Z]));
    if (out.v[X] != out.v[X] || out.v[Y] != out.v[Y] || out.v[Z] != out.v[Z]) {
        fprintf(stderr, "dir: %f %f %f theta0:%f theta1:%f before: %f %f %f after: %f %f %f\n", dir.v[X], dir.v[Y], dir.v[Z], theta0, theta1, bef.v[X], bef.v[Y], bef.v[Z], out.v[X], out.v[Y], out.v[Z]);
    }
    return (out);
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
