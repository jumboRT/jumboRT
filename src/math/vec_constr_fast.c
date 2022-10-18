#include "rtmath.h"

#if defined RT_VECTORIZE

t_vec
	vec(float x, float y, float z, float w)
{
	t_vec_conv	conv;

	conv.elem[0] = x;
	conv.elem[1] = y;
	conv.elem[2] = z;
	conv.elem[3] = w;
	return (conv.vec);
}

t_vec
	vec3(float x, float y, float z)
{
	return (vec(x, y, z, 0.0));
}

t_vec
	vec_0(void)
{
	return (vec(0.0, 0.0, 0.0, 0.0));
}

t_vec
	vec_x(float x)
{
	return (vec(x, 0.0, 0.0, 0.0));
}

t_vec
	vec_y(float y)
{
	return (vec(0.0, y, 0.0, 0.0));
}

t_vec
	vec_z(float z)
{
	return (vec(0.0, 0.0, z, 0.0));
}

t_vec
	vec_w(float w)
{
	return (vec(0.0, 0.0, 0.0, w));
}
#endif
