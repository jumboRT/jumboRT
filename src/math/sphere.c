#include "rtmath.h"

__attribute__ ((const))
t_sphere
	sphere(t_vec pos, FLOAT radius)
{
	t_sphere	result;

	result.pos = pos;
	result.radius = radius;
	return (result);
}

__attribute__ ((const))
static t_vec2
	sphere_uv_at(t_vec point)
{
	/* TODO Check if better to multiply by constant
	 * 1.0 / RT_2PI instead of dividing*/
	return (vec2(
		    0.5 + (rt_atan2(x(point), y(point)) / RT_2PI),
		    0.5 + (rt_asin(-z(point) / RT_2PI))));
}

/* https://raytracing.github.io/books/RayTracingInOneWeekend.html */
int
	ray_sphere_intersect(t_ray ray, t_sphere sphere, FLOAT min, t_hit *hit)
{
	t_quadratic	quadratic;
	t_vec		oc;
	FLOAT		t[2];

	oc = vec_sub(ray.org, sphere.pos);
	quadratic.a = 1.0;
	quadratic.b = 2.0 * vec_dot(oc, ray.dir);
	quadratic.c = vec_mag2(oc) - (sphere.radius * sphere.radius);
	if (quadratic_solve(&quadratic, t) == 0)
		return (0);
	if (t[1] < t[0] && t[1] >= min)
		t[0] = t[1];
	else if (t[0] < min)
		return (0);
	hit->t = t[0];
	hit->pos = ray_at(ray, t[0]);
	hit->normal = vec_scale(vec_sub(hit->pos, sphere.pos), 1.0 / sphere.radius);
	hit->uv = sphere_uv_at(hit->normal);
	return (1);
}

