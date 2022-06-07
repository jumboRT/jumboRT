#include "scene.h"

#include <math.h>
#include <stdlib.h>

const t_entity_vt
	*plane_vt(void)
{
	static const t_entity_vt	vt = {
		plane_hit,
		plane_destroy,
		plane_compare,
		plane_get_bounds
	};

	return (&vt);
}

int
	ray_plane_intersect(t_ray ray, t_vec pos, t_vec normal, FLOAT *t)
{
	FLOAT			dividend;
	FLOAT			divisor;

	divisor = vec_dot(ray.dir, normal);
	if (float_eq(divisor, 0, 0.000001))
		return (0);
	dividend = vec_dot(vec_sub(pos, ray.pos), normal);
	*t = dividend / divisor;
	if (*t < 0)
		return (0);
	return (1);
}

/*
static t_vec
	plane_uv_at(t_vec up, t_vec normal, t_vec point, t_vec uv_repeat)
{
	t_vec	perp0, perp1;

	perp0 = vec_norm(point);
	perp1 = vec_norm(vec_cross(normal, perp0));

}
*/

/* http://lousodrome.net/blog/light/2020/07/03/intersection-of-a-ray-and-a-plane/ */
/* http://wscg.zcu.cz/wscg2002/Papers_2002/A83.pdf */
int
	plane_hit(const t_entity *ent, t_ray ray, t_hit *hit, FLOAT min)
{
	const t_plane	*plane;
	FLOAT			dividend;
	FLOAT			divisor;
	FLOAT			t;

	plane = (const t_plane *) ent;
	divisor = vec_dot(ray.dir, plane->dir);
	if (float_eq(divisor, 0, 0.001))
		return (0);
	dividend = vec_dot(vec_sub(plane->pos, ray.pos), plane->dir);
	t = dividend / divisor;
	if (t < min)
		return (0);
	hit->t = t;
	hit->pos = ray_at_t(ray, t);
	hit->normal = plane->dir;
	hit->mat = plane->mat;
	return (1);
}

void
	plane_destroy(t_entity *ent)
{
	t_plane	*plane;

	plane = (t_plane *) ent;
	plane->mat->vt->destroy(plane->mat);
	rt_free(ent);
}

int
	plane_compare(t_entity *ent, t_vec pos, t_vec dir)
{
	(void) ent;
	(void) pos;
	(void) dir;
	return (0);
}

int
	plane_get_bounds(const t_entity *ent, t_vec *a, t_vec *b)
{
	const t_plane	*plane;

	plane = (const t_plane *) ent;
	*a = plane->pos;
	*b = plane->pos;
	return (0);
}

