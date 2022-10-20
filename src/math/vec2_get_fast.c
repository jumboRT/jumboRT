#include "rtmath.h"

#if defined RT_VECTORIZE

float
	u(t_vec2 v)
{
	t_vec2_conv	conv;

	conv.vec = v;
	return (conv.elem[0]);
}

float
	v(t_vec2 v)
{
	t_vec2_conv	conv;

	conv.vec = v;
	return (conv.elem[1]);
}

#endif
