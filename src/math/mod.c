#include "rtmath.h"

#ifndef RT_OPENCL

# include <math.h>

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

