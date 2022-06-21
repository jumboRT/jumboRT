#include "rtmath.h"

#ifndef RT_OPENCL

# include <math.h>

FLOAT
	rt_sin(FLOAT a)
{
	return (sinf(a));
}

#else

FLOAT
	rt_sin(FLOAT a)
{
	return (sin(a));
}

#endif

