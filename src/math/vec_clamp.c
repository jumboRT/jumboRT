#include "rtmath.h"

#if !defined RT_VECTORIZE

__attribute__ ((const))
t_vec
	vec_min(t_vec a, t_vec b)
{
	a.x = rt_min(a.x, b.x);
	a.y = rt_min(a.y, b.y);
	a.z = rt_min(a.z, b.z);
	return (a);
}

__attribute__ ((const))
t_vec
	vec_max(t_vec a, t_vec b)
{
	a.x = rt_max(a.x, b.x);
	a.y = rt_max(a.y, b.y);
	a.z = rt_max(a.z, b.z);
	return (a);
}

__attribute__ ((const))
t_vec
	vec_clamp(t_vec v, FLOAT min, FLOAT max)
{
	v.x = rt_max(rt_min(v.x, max), min);
	v.y = rt_max(rt_min(v.y, max), min);
	v.z = rt_max(rt_min(v.z, max), min);
	return (v);
}

#endif
