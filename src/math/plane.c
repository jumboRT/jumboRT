#include "rtmath.h"

t_plane
	plane(t_vec pos, t_vec normal)
{
	t_plane	result;

	result.pos = pos;
	result.normal = normal;
	return (result);
}

static t_vec
	plane_hit_get_tanget(t_plane plane, t_vec point)
{
	t_vec	op;

	op = vec_sub(plane.pos, point);
	if (rt_abs(z(plane.normal)) >= 0.999)
		return (vec3(z(plane.normal), -x(plane.normal), 0.0));
	return (vec3(y(plane.normal), -x(plane.normal), 0.0));
	return (vec_norm(vec_cross(vec_norm(op), plane.normal)));
}

static t_vec2
	plane_get_uv(t_plane plane, t_vec point, t_vec up, t_vec right)
{
	t_vec	op;

	op = vec_sub(plane.pos, point);
	return (vec2(vec_dot(op, up), vec_dot(op, right)));
	return (vec2(x(op), y(op)));
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
	hit->dpdu = plane_hit_get_tanget(plane, hit->pos);
	hit->dpdv = vec_norm(vec_cross(plane.normal, hit->dpdu));
	hit->uv = plane_get_uv(plane, hit->pos, hit->dpdu, hit->dpdv);
	return (t >= min);
}
