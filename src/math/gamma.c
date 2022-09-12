#include "rtmath.h"

float
	rt_gc(float arg)
{
	if (arg <= 0.0031308f)
		return (arg * 12.92f);
	return (rt_pow(arg, 1.0f / 2.4f) * 1.055f - 0.055f);
}

float
	rt_igc(float arg)
{
	if (arg <= 0.04045f)
		return (arg / 12.92f);
	return (rt_pow((arg + 0.055f) / 1.055f, 2.4f));
}

t_vec
	vec_gc(t_vec v)
{
	return (vec(rt_gc(x(v)), rt_gc(y(v)), rt_gc(z(v)), w(v)));
}

t_vec
	vec_igc(t_vec v)
{
	return (vec(rt_igc(x(v)), rt_igc(y(v)), rt_igc(z(v)), w(v)));
}
