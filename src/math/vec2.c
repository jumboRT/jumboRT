#include "rtmath.h"

#if !defined RT_VECTORIZE

t_vec2
	vec2(float x, float y)
{
	t_vec2	result;

	result.x = x;
	result.y = y;
	return (result);
}

#endif
