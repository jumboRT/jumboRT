#include "rtmath.h"

#ifndef RT_OPENCL

# include <math.h>

FLOAT
	rt_cos(FLOAT a)
{
	return (cosf(a));
}

FLOAT
	rt_acos(FLOAT a)
{
	return (acosf(a));
}

#else

FLOAT
	rt_cos(FLOAT a)
{
	return (cos(a));
}

FLOAT
	rt_acos(FLOAT a)
{
	return (acos(a));
}

#endif

