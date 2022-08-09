#include "rtmath.h"

#ifndef RT_OPENCL

# include <math.h>

FLOAT
	rt_sqrt(FLOAT a)
{
	return (sqrtf(a));
}

FLOAT
	rt_cbrt(FLOAT a)
{
	return (cbrtf(a));
}

#else

FLOAT
	rt_sqrt(FLOAT a)
{
	return (sqrt(a));
}

FLOAT
	rt_cbrt(FLOAT a)
{
	return (cbrt(a));
}

#endif

