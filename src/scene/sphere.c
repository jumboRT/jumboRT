#include "scene.h"

#include <math.h>
#include <stdlib.h>

const t_entity_vt
	*sphere_vt(void)
{
	static const t_entity_vt	vt = {
		sphere_hit,
		sphere_destroy,
		sphere_compare,
		sphere_get_pos
	};

	return (&vt);
}

/* https://raytracing.github.io/books/RayTracingInOneWeekend.html */
/* TODO: rewrite this function so it's not just copied from ^^^ */
int
	sphere_hit(t_entity *ent, t_ray ray, t_hit *hit, FLOAT min)
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
	if (root < min)
	{
		root = (-half_b + sqrtd) / a;
		if (root < min)
			return (0);
	}
	hit->t = root;
	hit->pos = vec_add(ray.pos, vec_scale(ray.dir, hit->t));
	hit->normal = vec_scale(vec_sub(hit->pos, sphere->pos), 2 / sphere->diameter);
	hit->mat = sphere->mat;
	return (1);
}

void
	sphere_destroy(t_entity *ent)
{
	t_sphere	*sphere;

	sphere = (t_sphere *) ent;
	sphere->mat->vt->destroy(sphere->mat);
	rt_free(ent);
}

int
	sphere_compare(t_entity *ent, t_vec pos, t_vec dir)
{
	t_sphere	*sphere;

	sphere = (t_sphere *) ent;
	return (sphere_plane_compare(pos, dir, sphere->pos, sphere->diameter / 2));
}

t_vec
	sphere_get_pos(const t_entity *ent)
{
	const t_sphere	*sphere;

	sphere = (const t_sphere *) ent;
	return (sphere->pos);
}

