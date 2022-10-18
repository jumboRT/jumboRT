#include "rtmath.h"

#ifndef RT_OPENCL

float
	rt_pow(float x, float y)
{
	return (powf(x, y));
}

#else

float
	rt_pow(float x, float y)
{
	return (pow(x, y));
}

#endif
