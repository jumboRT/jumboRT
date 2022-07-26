#include "rtmath.h"

#if defined RT_VECTORIZE

t_vec2
	vec2(FLOAT x, FLOAT y)
{
	t_vec2_conv	conv;

	conv.elem[0] = x;
	conv.elem[1] = y;
	return (conv.vec);
}
	
#endif
