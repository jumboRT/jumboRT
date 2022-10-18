#include "rtmath.h"

#if defined RT_VECTORIZE

t_vec2
	vec2(float x, float y)
{
	t_vec2_conv	conv;

	conv.elem[0] = x;
	conv.elem[1] = y;
	return (conv.vec);
}

t_vec2
	vec2_0(void)
{
	return (vec2(0.0f, 0.0f));
}

#endif
