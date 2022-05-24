#include "rtmath.h"

t_vec
	color_mul(t_vec a, t_vec b)
{
	return (vec(
			a.v[X] * b.v[X],
			a.v[Y] * b.v[Y],
			a.v[Z] * b.v[Z],
			a.v[W] * b.v[W]));
}
