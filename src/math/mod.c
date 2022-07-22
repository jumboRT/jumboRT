#include "rtmath.h"

#ifndef RT_OPENCL

# include <math.h>

FLOAT
	rt_mod(FLOAT a, FLOAT b)
{
	return (fmod(a, b));
}

#else

FLOAT
	rt_mod(FLOAT a, FLOAT b)
{
	return (fmod(a, b));
}

#endif

