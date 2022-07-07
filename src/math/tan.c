#include "rtmath.h"

#ifndef RT_OPENCL

# include <math.h>

FLOAT
	rt_tan(FLOAT a)
{
	return (tanf(a));
}

FLOAT
	rt_atan(FLOAT a)
{
	return (atanf(a));
}

FLOAT
	rt_atan2(FLOAT y, FLOAT x)
{
	return (atan2f(y, x));
}

#else

FLOAT
	rt_tan(FLOAT a)
{
	return (tan(a));
}

float
	rt_atan(float a)
{
	return (atan(a));
}

FLOAT
	rt_atan2(FLOAT y, FLOAT x)
{
	return (atan2(y, x));
}

#endif

