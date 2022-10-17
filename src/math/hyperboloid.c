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

void
	hyperboloid_hit_info(t_ray ray, t_hyperboloid hyperboloid, t_hit *hit)
{
	(void) ray;
	(void) hyperboloid;
	(void) hit;
	hit->dpdu = vec_tangent(hit->geometric_normal);
	hit->dpdv = vec_norm(vec_cross(hit->dpdu, hit->geometric_normal));
	hit->uv = vec2(x(hit->pos), y(hit->pos));
}

/* https://www.cs.uaf.edu/2012/spring/cs481/section/0/lecture/01_26_ray_intersections.html */
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
	hit->geometric_normal =
		vec_norm(vec3(
			-2.0f * rt_pow(hyperboloid.a, -2.0f) * x(hit->pos),
			-2.0f * rt_pow(hyperboloid.b, -2.0f) * y(hit->pos),
			2.0f * rt_pow(hyperboloid.c, -2.0f) * z(hit->pos)));
	if ((z(hit->geometric_normal) > 0
		&& z(vec_sub(hit->pos, hyperboloid.pos)) > 0) ||
		(z(hit->geometric_normal) < 0
		 && z(vec_sub(hit->pos, hyperboloid.pos)) < 0))
		hit->geometric_normal = vec_neg(hit->geometric_normal);	
	hit->shading_normal = hit->geometric_normal;
	return (1);
}

