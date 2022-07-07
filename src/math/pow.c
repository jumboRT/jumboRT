#include "rtmath.h"

#ifndef RT_OPENCL

# include <math.h>

FLOAT
	rt_pow(FLOAT x, FLOAT y)
{
	return (powf(x, y));
}

#else

FLOAT
	rt_pow(FLOAT x, FLOAT y)
{
	return (pow(x, y));
}

#endif
