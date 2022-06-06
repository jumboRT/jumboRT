#include "scene.h"

#include <math.h>
#include <stdlib.h>

#define LIGHT_RADIUS 0.5

const t_entity_vt
	*light_vt(void)
{
	static const t_entity_vt	vt = {
		light_hit,
		light_destroy,
		light_compare,
		light_get_pos
	};

	return (&vt);
}

/* https://raytracing.github.io/books/RayTracingInOneWeekend.html */
/* TODO: rewrite this function so it's not just copied from ^^^ */
int
	light_hit(const t_entity *ent, t_ray ray, t_hit *hit, FLOAT min)
{
	t_light *const	light = (t_light *) ent;
	const t_vec		oc = vec_sub(ray.pos, light->pos);
	const FLOAT		a = vec_mag2(ray.dir);
	const FLOAT		half_b = vec_dot(oc, ray.dir);
	const FLOAT		c = vec_mag2(oc) - pow(LIGHT_RADIUS, 2);
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
	hit->normal = vec_norm(vec_sub(hit->pos, light->pos));
	hit->mat = light->mat;
	return (1);
}

void
	light_destroy(t_entity *ent)
{
	t_light	*light;

	light = (t_light *) ent;
	light->mat->vt->destroy(light->mat);
	rt_free(ent);
}

int
	light_compare(t_entity *ent, t_vec pos, t_vec dir)
{
	t_light	*light;

	light = (t_light *) ent;
	return (sphere_plane_compare(pos, dir, light->pos, LIGHT_RADIUS));
}

t_vec
	light_get_pos(const t_entity *ent)
{
	const t_light	*light;

	light = (const t_light *) ent;
	return (light->pos);
}

