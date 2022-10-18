#include "rtmath.h"

#if defined RT_VECTORIZE

float
	vec_dot(t_vec a, t_vec b)
{
	t_vec_conv	conv;

	conv.vec = a * b;
	return (conv.elem[0] + conv.elem[1] + conv.elem[2]);
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
