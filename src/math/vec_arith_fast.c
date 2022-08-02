#include "rtmath.h"

#if defined RT_VECTORIZE

t_vec
	vec_neg(t_vec v)
{
	return (-v);
}

t_vec
	vec_scale(t_vec v, FLOAT s)
{
	t_vec	b;

	b = vec(s, s, s, s);
	return (v * b);
}

t_vec
	vec_add(t_vec a, t_vec b)
{
	return (a + b);
}

t_vec
	vec_sub(t_vec a, t_vec b)
{
	return (a - b);
}

t_vec
	vec_mul(t_vec a, t_vec b)
{
	return (a * b);
}

#endif
