#include "rtmath.h"

#ifndef RT_OPENCL

# include <math.h>

FLOAT
	rt_sqrt(FLOAT a)
{
	return (sqrtf(a));
}

#else

FLOAT
	rt_sqrt(FLOAT a)
{
	return sqrt(a);
}

#endif

