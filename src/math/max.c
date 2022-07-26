#include "rtmath.h"

#ifndef RT_OPENCL

# include <math.h>

FLOAT
	rt_max(FLOAT a, FLOAT b)
{
	return (fmax(a, b));
}

#else

FLOAT
	rt_max(FLOAT a, FLOAT b)
{
	return (fmax(a, b));
}

#endif

