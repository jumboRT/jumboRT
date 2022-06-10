#include "rtmath.h"

#if !defined RT_SSE

__attribute__ ((const))
FLOAT
	vec_dot(t_vec a, t_vec b)
{
	return (0
			+ x(a) * x(b)
			+ y(a) * y(b)
			+ z(a) * z(b));
}

__attribute__ ((const))
t_vec
	vec_cross(t_vec a, t_vec b)
{
	return (vec(
			(y(a) * z(b)) - (z(a) * y(b)),
			(z(a) * x(b)) - (x(a) * z(b)),
			(x(a) * y(b)) - (y(a) * x(b))));
}
#endif
