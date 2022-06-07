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
		sphere_get_bounds
	};

	return (&vt);
}

/* https://raytracing.github.io/books/RayTracingInOneWeekend.html */
int
	sphere_hit(const t_entity *ent, t_ray ray, t_hit *hit, FLOAT min)
{
	const t_sphere	*sphere;
	t_quadratic		quadratic;
	t_vec			oc;
	FLOAT			t[2];

	sphere = (const t_sphere *) ent;
	oc = vec_sub(ray.pos, sphere->pos);
	quadratic.a = vec_mag2(ray.dir);
	quadratic.b = 2.0 * vec_dot(oc, ray.dir);
	quadratic.c = vec_mag2(oc) - pow(sphere->diameter / 2.0, 2.0);
	if (quadratic_solve(&quadratic, t) == 0)
		return (0);
	if (t[0] < min)
		t[0] = t[1];
	if (t[0] < min)
		return (0);
	hit->t = t[0];
	hit->pos = ray_at_t(ray, t[0]);
	hit->normal = vec_scale(vec_sub(hit->pos, sphere->pos), 2.0 / sphere->diameter);
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

int
	sphere_get_bounds(const t_entity *ent, t_vec *a, t_vec *b)
{
	const t_sphere	*sphere;
	FLOAT			radius;

	sphere = (const t_sphere *) ent;
	radius = sphere->diameter / 2;
	*a = vec_sub(sphere->pos, vec(radius, radius, radius, 0));
	*b = vec_add(sphere->pos, vec(radius, radius, radius, 0));
	return (1);
}

