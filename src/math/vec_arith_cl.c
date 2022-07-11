#include "rtmath.h"

#if defined RT_OPENCL

__attribute__ ((const))
t_vec
	vec_neg(t_vec v)
{
	return (v * -1);
}

__attribute__ ((const))
t_vec
	vec_scale(t_vec v, FLOAT s)
{
	return (v * s);
}

__attribute__ ((const))
t_vec
	vec_add(t_vec a, t_vec b)
{
	return (a + b);
}

__attribute__ ((const))
t_vec
	vec_sub(t_vec a, t_vec b)
{
	return (a - b);
}

__attribute__ ((const))
t_vec
	vec_mul(t_vec a, t_vec b)
{
	return (a * b);
}

#endif
