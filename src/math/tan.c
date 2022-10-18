#include "rtmath.h"
#include <math.h>

#ifndef RT_OPENCL

float
	rt_tan(float a)
{
	return (tanf(a));
}

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
	rt_tan(float a)
{
	return (tan(a));
}

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
