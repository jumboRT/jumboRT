#include "rtmath.h"

#ifndef RT_OPENCL

# include <math.h>

FLOAT
	rt_exp(FLOAT arg)
{
	return (exp(arg));
}

#else

FLOAT
	rt_exp(FLOAT arg)
{
	return (exp(arg));
}

#endif
