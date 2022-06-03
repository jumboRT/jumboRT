#include "rtmath.h"

#include <math.h>

t_vec
	vec_min(t_vec a, t_vec b)
{
	return vec(
			fmin(a.v[X], b.v[X]),
			fmin(a.v[Y], b.v[Y]),
			fmin(a.v[Z], b.v[Z]),
			fmin(a.v[W], b.v[W]));
}

t_vec
	vec_max(t_vec a, t_vec b)
{
	return vec(
			fmax(a.v[X], b.v[X]),
			fmax(a.v[Y], b.v[Y]),
			fmax(a.v[Z], b.v[Z]),
			fmax(a.v[W], b.v[W]));
}

