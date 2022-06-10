#include "rtmath.h"

#if !defined RT_SSE

t_vec
	vec(FLOAT x, FLOAT y, FLOAT z)
{
	t_vec	result;

	result.x = x;
	result.y = y;
	result.z = z;
	return (result);
}
	
t_vec
	vec_0(void)
{
	return (vec(0.0, 0.0, 0.0));
}

t_vec
	vec_x(FLOAT x)
{
	return (vec(x, 0.0, 0.0));
}

t_vec
	vec_y(FLOAT y)
{
	return (vec(0.0, y, 0.0));
}

t_vec
	vec_z(FLOAT z)
{
	return (vec(0.0, 0.0, z));
}
#endif
