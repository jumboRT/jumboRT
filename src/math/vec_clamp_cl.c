#include "rtmath.h"

#if defined RT_OPENCL

__attribute__ ((const))
t_vec
	vec_min(t_vec a, t_vec b)
{
	return (fmin(a, b));
}

__attribute__ ((const))
t_vec
	vec_max(t_vec a, t_vec b)
{
	return (fmax(a, b));
}

__attribute__ ((const))
t_vec
	vec_clamp(t_vec v, FLOAT min, FLOAT max)
{
	return (clamp(v, min, max));
}

#endif
