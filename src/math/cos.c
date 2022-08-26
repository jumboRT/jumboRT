#include "rtmath.h"

#ifndef RT_OPENCL

# include <math.h>

float
	rt_cos(float a)
{
	return (cosf(a));
}

float
	rt_acos(float a)
{
	return (acosf(a));
}

#else

float
	rt_cos(float a)
{
	return (cos(a));
}

float
	rt_acos(float a)
{
	return (acos(a));
}

#endif

