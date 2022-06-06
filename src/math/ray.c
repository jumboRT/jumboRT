#include "rtmath.h"

t_vec
	ray_at_t(t_ray ray, FLOAT t)
{
	return (vec_add(ray.pos, vec_scale(ray.dir, t)));
}
