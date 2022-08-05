#include "rtmath.h"

#ifndef RT_OPENCL

# include <math.h>

FLOAT
	rt_log(FLOAT arg)
{
	return (log(arg));
}

#else

FLOAT
	rt_log(FLOAT arg)
{
	return (log(arg));
}

#endif

