#include "rtmath.h"

t_hyperboloid
	hyperboloid(t_vec pos, float a, float b, float c)
{
	t_hyperboloid	result;

	result.pos = pos;
	result.a = a;
	result.b = b;
	result.c = c;
	return (result);
}

/* TODO: compute hyperboloid hit info */
void
	hyperboloid_hit_info(t_ray ray, t_hyperboloid hyperboloid, t_hit *hit)
{
	(void) ray;
	(void) hyperboloid;
	(void) hit;
	hit->dpdu = vec_0();
	hit->dpdv = vec_0();
	hit->uv = vec2(0, 0);
}

int
	ray_hyperboloid_intersect(t_ray ray, t_hyperboloid hyperboloid, float min, t_hit *hit)
{
	t_quadratic	quadratic;
	t_vec		oc;
	t_vec		dir;
	float		t[2];

	oc = vec_sub(ray.org, hyperboloid.pos);
	oc = vec(x(oc) / hyperboloid.a, y(oc) / hyperboloid.b, z(oc) / hyperboloid.c, 0);
	dir = vec(x(ray.dir) / hyperboloid.a, y(ray.dir) / hyperboloid.b, z(ray.dir) / hyperboloid.c, 0);
	quadratic.a = x(dir) * x(dir) + y(dir) * y(dir) - z(dir) * z(dir);
	quadratic.b = 2 * (x(oc) * x(dir) + y(oc) * y(dir) - z(oc) * z(dir));
	quadratic.c = x(oc) * x(oc) + y(oc) * y(oc) - z(oc) * z(oc) + 1;
	if (quadratic_solve(&quadratic, t) == 0)
		return (0);
	if (t[0] < 0)
		t[0] = t[1];
	if (t[1] < t[0] && t[1] >= min)
		t[0] = t[1];
	else if (t[0] < min)
		return (0);
	hit->t = t[0];
	hit->pos = ray_at(ray, t[0]);
	// TODO: compute normal 
	hit->geometric_normal = vec(0, 0, 1, 0);
	hit->shading_normal = vec(0, 0, 1, 0);
	return (1);
}

