#include "rtmath.h"

t_vec
	vec_0(void)
{
	return (vec(0, 0, 0, 0));
}

t_vec
	vec_x(FLOAT v)
{
	return (vec(v, 0, 0, 0));
}

t_vec
	vec_y(FLOAT v)
{
	return (vec(0, v, 0, 0));
}

t_vec
	vec_z(FLOAT v)
{
	return (vec(0, 0, v, 0));
}

t_vec
	vec_w(FLOAT v)
{
	return (vec(0, 0, 0, v));
}

