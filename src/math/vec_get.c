#include "rtmath.h"

#if !defined RT_VECTORIZE

float
	x(t_vec v)
{
	return (v.x);
}

float
	y(t_vec v)
{
	return (v.y);
}

float
	z(t_vec v)
{
	return (v.z);
}

float
	w(t_vec v)
{
	return (v.w);
}

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

float
	xyz(t_vec v, int i)
{
	if (i == 0)
		return (x(v));
	else if (i == 1)
		return (y(v));
	else
		return (z(v));
}

int
	vec_eq(t_vec a, t_vec b)
{
	return (x(a) == x(b) && y(a) == y(b) && z(a) == z(b));
}

int
	vec2_eq(t_vec2 a, t_vec2 b)
{
	return (u(a) == u(b) && v(a) == v(b));
}

#endif
