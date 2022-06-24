#include "rtmath.h"

#ifndef RT_OPENCL

# include <math.h>

FLOAT
	rt_min(FLOAT a, FLOAT b)
{
	return (fmin(a, b));
}

#else

FLOAT
	rt_min(FLOAT a, FLOAT b)
{
	return (fmin(a, b));
}

#endif

