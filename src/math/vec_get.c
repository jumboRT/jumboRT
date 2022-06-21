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
	z(t_vec v)
{
	return (0.0);
}
#endif
