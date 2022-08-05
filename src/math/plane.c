#include "rtmath.h"

t_plane
	plane(t_vec pos, t_vec normal)
{
	t_plane	result;

	result.pos = pos;
	result.normal = normal;
	return (result);
}

int
	ray_plane_intersect(t_ray ray, t_plane plane, FLOAT min, t_hit *hit)
{
	FLOAT	divisor;
	FLOAT	dividend;
	FLOAT	t;

	divisor = vec_dot(ray.dir, plane.normal);
	if (divisor == 0.0)
		return (0);
	dividend = vec_dot(vec_sub(plane.pos, ray.org), plane.normal);
	t = dividend / divisor;
	hit->t = t;
	hit->pos = ray_at(ray, t);
	hit->normal = plane.normal;
	return (t >= min);
}
