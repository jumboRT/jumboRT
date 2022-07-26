#include "rtmath.h"

#if !defined RT_VECTORIZE

__attribute__ ((const))
FLOAT
	x(t_vec v)
{
	return (v.x);
}

__attribute__ ((const))
FLOAT
	y(t_vec v)
{
	return (v.y);
}

__attribute__ ((const))
FLOAT
	z(t_vec v)
{
	return (v.z);
}

__attribute__ ((const))
FLOAT
	xyz(t_vec v, int i)
{
	if (i == 0)
		return (x(v));
	else if (i == 1)
		return (y(v));
	else
		return (z(v));
}

__attribute__ ((const))
int
	vec_eq(t_vec a, t_vec b)
{
	return (x(a) == x(b) && y(a) == y(b) && z(a) == z(b));
}

#endif
