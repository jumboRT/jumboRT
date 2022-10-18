#include "rtmath.h"

#if !defined RT_VECTORIZE

float
	vec_dot(t_vec a, t_vec b)
{
	return (0 + x(a) * x(b) + y(a) * y(b) + z(a) * z(b));
}

t_vec
	vec_cross(t_vec a, t_vec b)
{
	return (vec(
			(y(a) * z(b)) - (z(a) * y(b)),
			(z(a) * x(b)) - (x(a) * z(b)),
			(x(a) * y(b)) - (y(a) * x(b)),
			0.0));
}

#endif
