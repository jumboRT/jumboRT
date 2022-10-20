#include "rtmath.h"

#ifndef RT_OPENCL

float
	rt_atan(float a)
{
	return (atanf(a));
}

float
	rt_atan2(float y, float x)
{
	return (atan2f(y, x));
}

#else

float
	rt_atan(float a)
{
	return (atan(a));
}

float
	rt_atan2(float y, float x)
{
	return (atan2(y, x));
}

#endif
