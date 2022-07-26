#include "rtmath.h"

__attribute__ ((const))
t_ray
	ray(t_vec org, t_vec dir)
{
	t_ray	result;

	result.org = org;
	result.dir = dir;
	return (result);
}

__attribute__ ((const))
t_vec
	ray_at(t_ray ray, FLOAT t)
{
	return (vec_add(ray.org, vec_scale(ray.dir, t)));
}
