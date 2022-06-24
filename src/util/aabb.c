#include "aabb.h"

t_bounds
	bounds(t_vec min, t_vec max)
{
	t_bounds	bounds;

	bounds.min = min;
	bounds.max = max;
	return (bounds);
}

t_bounds
	bounds_0(void)
{
	t_vec	min;

	min = vec(RT_HUGE_VAL, RT_HUGE_VAL, RT_HUGE_VAL);
	return (bounds(min, vec_neg(min)));
}

t_bounds
	bounds_max(t_bounds a, t_bounds b)
{
	return (bounds(vec_min(a.min, b.min), vec_max(a.max, b.max)));
}

FLOAT
	bounds_surf(t_bounds a)
{
	t_vec	s;

	s = vec_sub(a.max, a.min);
	if (x(s) <= 0 || y(s) <= 0 || z(s) <= 0)
		return (0);
	return (x(s) * y(s) * 2 + x(s) * z(s) * 2 + y(s) * z(s) * 2);
}

