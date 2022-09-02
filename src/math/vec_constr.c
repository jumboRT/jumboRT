#include "rtmath.h"

#if !defined RT_VECTORIZE

t_vec
	vec(float x, float y, float z, float w)
{
	t_vec	result;

	result.x = x;
	result.y = y;
	result.z = z;
	result.w = w;
	return (result);
}

t_vec
	vec3(float x, float y, float z)
{
	t_vec	result;

	result.x = x;
	result.y = y;
	result.z = z;
	result.w = 0;
	return (result);
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
#endif
