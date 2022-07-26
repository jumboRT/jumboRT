#include "rtmath.h"

#if defined RT_VECTORIZE

__attribute__ ((const))
t_vec
	vec_neg(t_vec v)
{
	return (-v);
}

__attribute__ ((const))
t_vec
	vec_scale(t_vec v, FLOAT s)
{
	t_vec	b;

	b = vec(s, s, s, s);
	return (v * b);
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
