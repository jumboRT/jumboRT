#include "rtmath.h"
#include <math.h>

#ifndef RT_OPENCL

float
	rt_mod(float a, float b)
{
	return (fmod(a, b));
}

#else

float
	rt_mod(float a, float b)
{
	return (fmod(a, b));
}

#endif
