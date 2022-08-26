#include "rtmath.h"

#ifndef RT_OPENCL

# include <math.h>

float
	rt_min(float a, float b)
{
	return (fmin(a, b));
}

#else

float
	rt_min(float a, float b)
{
	return (fmin(a, b));
}

#endif

