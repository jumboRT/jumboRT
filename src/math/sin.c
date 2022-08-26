#include "rtmath.h"

#ifndef RT_OPENCL

# include <math.h>

float
	rt_sin(float a)
{
	return (sinf(a));
}

float
	rt_asin(float a)
{
	return (asinf(a));
}

#else

float
	rt_sin(float a)
{
	return (sin(a));
}

float
	rt_asin(float a)
{
	return (asin(a));
}

#endif

