#include "rtmath.h"

#ifndef RT_OPENCL

float
	rt_abs(float a)
{
	return (fabs(a));
}

#else

float
	rt_abs(float a)
{
	return (fabs(a));
}

#endif
