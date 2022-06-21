#include "rtmath.h"

#ifndef RT_OPENCL

# include <math.h>

FLOAT
	rt_cos(FLOAT a)
{
	return (cosf(a));
}

#else

FLOAT
	rt_cos(FLOAT a)
{
	return (cos(a));
}

#endif

