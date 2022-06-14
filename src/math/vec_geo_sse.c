#include "rtmath.h"

#if defined RT_SSE
#include <smmintrin.h>

__attribute__ ((const))
FLOAT
	vec_dot(t_vec a, t_vec b)
{
	FLOAT	res[4];
	__m128	dot;

	dot = _mm_dp_ps(a.v, b.v, -1);
	_mm_store_ps(res, dot);
	return (res[0]);
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
