#include "rtmath.h"

#include <math.h>

FLOAT
	vec_dot(t_vec a, t_vec b)
{
	return (0
		+ a.v[X] * b.v[X]
		+ a.v[Y] * b.v[Y]
		+ a.v[Z] * b.v[Z]
		+ a.v[W] * b.v[W]);
}

FLOAT
	vec_mag(t_vec a)
{
	return (sqrt(vec_mag2(a)));
}

FLOAT
	vec_mag2(t_vec a)
{
	return (vec_dot(a, a));
}

t_vec
	vec_scale(t_vec a, FLOAT b)
{
	return (vec(
			a.v[X] * b,
			a.v[Y] * b,
			a.v[Z] * b,
			a.v[W] * b));
}
