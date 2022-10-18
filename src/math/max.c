#include "rtmath.h"
#include <math.h>

#ifndef RT_OPENCL

float
	rt_max(float a, float b)
{
	return (fmax(a, b));
}

#else

float
	rt_max(float a, float b)
{
	return (fmax(a, b));
}

#endif
