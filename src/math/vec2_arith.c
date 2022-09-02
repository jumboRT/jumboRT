#include "rtmath.h"

#if !defined RT_VECTORIZE


__attribute__ ((const))
t_vec2
	vec2_neg(t_vec2 v)
{
	t_vec2	result;

	result.x = -v.x;
	result.y = -v.y;
	return (result);
}

__attribute__ ((const))
t_vec2
	vec2_scale(t_vec2 v, float s)
{
	t_vec2	result;

	result.x = v.x * s;
	result.y = v.y * s;
	return (result);
}

__attribute__ ((const))
t_vec2
	vec2_add(t_vec2 a, t_vec2 b)
{
	t_vec2	result;

	result.x = a.x + b.x;
	result.y = a.y + b.y;
	return (result);
}

__attribute__ ((const))
t_vec2
	vec2_sub(t_vec2 a, t_vec2 b)
{
	t_vec2	result;

	result.x = a.x - b.x;
	result.y = a.y - b.y;
	return (result);
}

__attribute__ ((const))
t_vec2
	vec2_mul(t_vec2 a, t_vec2 b)
{
	t_vec2	result;

	result.x = a.x * b.x;
	result.y = a.y * b.y;
	return (result);
}

#endif
