#include "rtmath.h"

#ifndef RT_OPENCL

# include <math.h>

float
	rt_exp(float arg)
{
	return (exp(arg));
}

#else

float
	rt_exp(float arg)
{
	return (exp(arg));
}

#endif
