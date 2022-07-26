#include "rtmath.h"

#ifndef RT_OPENCL

# include <math.h>

FLOAT
	rt_sin(FLOAT a)
{
	return (sinf(a));
}

FLOAT
	rt_asin(FLOAT a)
{
	return (asinf(a));
}

#else

FLOAT
	rt_sin(FLOAT a)
{
	return (sin(a));
}

FLOAT
	rt_asin(FLOAT a)
{
	return (asin(a));
}

#endif

