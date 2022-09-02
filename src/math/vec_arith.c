#include "rtmath.h"

#if !defined RT_VECTORIZE

t_vec
	vec_neg(t_vec v)
{
	return (vec(-x(v), -y(v), -z(v), -w(v)));
}

t_vec
	vec_scale(t_vec v, float s)
{
	return (vec(x(v) * s, y(v) * s, z(v) * s, w(v) * s));
}

t_vec
	vec_add(t_vec a, t_vec b)
{
	return (vec(x(a) + x(b), y(a) + y(b), z(a) + z(b), w(a) + w(b)));
}

t_vec
	vec_sub(t_vec a, t_vec b)
{
	return (vec_add(a, vec_neg(b)));
}

t_vec
	vec_mul(t_vec a, t_vec b)
{
	return (vec(x(a) * x(b), y(a) * y(b), z(a) * z(b), w(a) * w(b)));
}

#endif
