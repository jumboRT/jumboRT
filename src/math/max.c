#include "rtmath.h"

#ifndef RT_OPENCL

# include <math.h>

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

