#include "rtmath.h"

#if defined RT_OPENCL

__attribute__ ((const))
FLOAT vec_dot(t_vec a, t_vec b)
{
	return (dot(a, b));
}

__attribute__ ((const))
t_vec vec_cross(t_vec a, t_vec b)
{
	return (cross(a, b));
}

#endif
