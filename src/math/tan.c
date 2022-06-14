#include "rtmath.h"

#ifndef RT_OPENCL

# include <math.h>

FLOAT
	rt_tan(FLOAT a)
{
	return (tanf(a));
}

#else

FLOAT
	rt_tan(FLOAT a)
{
	return tan(a);
}

#endif

