#include "scene.h"
#include <math.h>

const t_entity_vt
	*sphere_vt(void)
{
	static const t_entity_vt	vt = {
		sphere_hit
	};

	return (&vt);
}

/* https://raytracing.github.io/books/RayTracingInOneWeekend.html */
int
	sphere_hit(t_entity *ent, t_ray ray, t_hit *hit)
{
	t_sphere *const	sphere = (t_sphere *) ent;
	const t_vec		oc = vec_sub(ray.pos, sphere->pos);
	const FLOAT		a = vec_mag2(ray.dir);
	const FLOAT		half_b = vec_dot(oc, ray.dir);
	const FLOAT		c = vec_mag2(oc) - pow(sphere->diameter / 2, 2);
	const FLOAT		discriminant = half_b * half_b - a * c;
	FLOAT			sqrtd;
	FLOAT			root;

	if (discriminant < 0)
		return (0);
	sqrtd = sqrt(discriminant);
	root = (-half_b - sqrtd) / a;
	if (root <= 0)
	{
		root = (-half_b + sqrtd) / a;
		if (root <= 0)
			return (0);
	}
	hit->t = root;
	hit->pos = vec_add(ray.pos, vec_scale(ray.dir, hit->t));
	hit->normal = vec_scale(vec_sub(hit->pos, sphere->pos), 2 / sphere->diameter);
	hit->color = sphere->color;
	return (1);
}
