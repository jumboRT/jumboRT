#include "rtmath.h"
#include "sample.h"

t_sphere
	sphere(t_vec pos, float radius)
{
	t_sphere	result;

	result.pos = pos;
	result.radius = radius;
	return (result);
}

t_vec2
	sphere_uv_at(t_vec point)
{
	return (vec2(
		    0.5 + (rt_atan2(x(point), y(point)) / RT_2PI),
		    0.5 + (rt_asin(z(point)) / RT_PI)));
}

void
	sphere_hit_info(t_ray ray, t_sphere sphere, t_hit *hit)
{
	float		cosphi, sinphi;
	float		theta;

	(void) ray;
	cosphi = x(hit->pos) / sphere.radius;
	sinphi = y(hit->pos) / sphere.radius;
	theta = rt_acos(rt_clamp(z(hit->pos) / sphere.radius, -1.0, 1.0));
	hit->dpdu = vec(-RT_2PI * y(hit->pos), RT_2PI * x(hit->pos), 0.0, 0.0);
	hit->dpdv = vec_scale(vec(z(hit->pos) * cosphi, z(hit->pos) * sinphi, -sphere.radius * rt_sin(theta), 0.0), RT_2PI);
	hit->uv = sphere_uv_at(hit->geometric_normal);
}

/* https://raytracing.github.io/books/RayTracingInOneWeekend.html */
int
	ray_sphere_intersect(t_ray ray, t_sphere sphere, float min, t_hit *hit)
{
	t_quadratic	quadratic;
	t_vec		oc;
	float		t[2];

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
	hit->geometric_normal = vec_norm(vec_sub(hit->pos, sphere.pos));
	hit->shading_normal = hit->geometric_normal;
	return (1);
}

t_vec
	sphere_sample(t_sphere sphere, GLOBAL t_context *ctx)
{
	return (vec_add(sphere.pos, vec_scale(vec_norm(rt_random_in_sphere(&ctx->seed)), sphere.radius)));
}

float
	sphere_area(t_sphere sphere)
{
	return (4.0f * RT_PI * sphere.radius * sphere.radius);
}

