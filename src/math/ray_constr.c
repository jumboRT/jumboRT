#include "rtmath.h"

t_ray
	ray(t_vec org, t_vec dir)
{
	t_ray	result;

	result.org = org;
	result.dir = dir;
	return (result);
}

t_vec
	ray_at(t_ray ray, float t)
{
	return (vec_add(ray.org, vec_scale(ray.dir, t)));
}
