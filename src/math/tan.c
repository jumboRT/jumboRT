#include "rtmath.h"

#ifndef RT_OPENCL

float
	rt_tan(float a)
{
	return (tanf(a));
}

#else

float
	rt_tan(float a)
{
	return (tan(a));
}
#endif
