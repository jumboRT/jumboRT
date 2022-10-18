#include "rtmath.h"

#ifndef RT_OPENCL

float
	rt_log(float arg)
{
	return (log(arg));
}

#else

float
	rt_log(float arg)
{
	return (log(arg));
}

#endif
