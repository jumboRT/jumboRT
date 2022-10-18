#include "rtmath.h"
#include <math.h>

#ifndef RT_OPENCL

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
