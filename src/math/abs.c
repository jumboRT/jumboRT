#include "rtmath.h"

#ifndef RT_OPENCL

# include <math.h>

FLOAT
	rt_abs(FLOAT a)
{
	return (fabs(a));
}

#else

FLOAT
	rt_abs(FLOAT a)
{
	return (fabs(a));
}

#endif

