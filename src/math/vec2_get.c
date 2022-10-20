#include "rtmath.h"

#if !defined RT_VECTORIZE

float
	u(t_vec2 v)
{
	return (v.x);
}

float
	v(t_vec2 v)
{
	return (v.y);
}

#endif
