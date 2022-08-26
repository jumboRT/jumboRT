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

#endif
