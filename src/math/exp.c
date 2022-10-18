#include "rtmath.h"

#ifndef RT_OPENCL

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
