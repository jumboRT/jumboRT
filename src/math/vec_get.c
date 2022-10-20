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
	xyz(t_vec v, int i)
{
	if (i == 0)
		return (x(v));
	else if (i == 1)
		return (y(v));
	else
		return (z(v));
}

#endif
