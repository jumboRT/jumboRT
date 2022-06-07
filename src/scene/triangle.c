#include "scene.h"

#include <math.h>
#include <stdlib.h>

const t_entity_vt
	*triangle_vt(void)
{
	static const t_entity_vt	vt = {
		triangle_hit,
		triangle_destroy,
		triangle_compare,
		triangle_get_bounds
	};

	return (&vt);
}

/* https://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates */
int
	triangle_hit(const t_entity *ent, t_ray ray, t_hit *hit, FLOAT min)
{
	const t_triangle	*triangle;
	FLOAT				t;
	t_vec				pos;
	t_vec				v2;
	FLOAT				uvw[3];
	FLOAT				d20, d21;

	triangle = (const t_triangle *) ent;
	if (!ray_plane_intersect(ray, triangle->pos0, triangle->normal, &t))
		return (0);
	if (t < min)
		return (0);
	pos = ray_at_t(ray, t);
	v2 = vec_sub(pos, triangle->pos0);
	d20 = vec_dot(v2, triangle->v0);
	d21 = vec_dot(v2, triangle->v1);
	uvw[1] = (triangle->d11 * d20 - triangle->d01 * d21) * triangle->inv_denom;
	uvw[2] = (triangle->d00 * d21 - triangle->d01 * d20) * triangle->inv_denom;
	uvw[0] = 1.0 - uvw[1] - uvw[2];
	if (uvw[0] < 0 || uvw[1] < 0 || uvw[2] < 0)
		return (0);
	if (!float_eq(uvw[0] + uvw[1] + uvw[2], 1.0, 0.001))
		return (0);
	hit->t = t;
	hit->pos = pos;
	hit->normal = triangle->normal;
	hit->mat = triangle->mat;
	hit->uv = vec_add(triangle->local_uv, vec(uvw[0], uvw[1], 0.0, 0.0));
	return (1);
}

void
	triangle_destroy(t_entity *ent)
{
	t_triangle	*triangle;

	triangle = (t_triangle *) ent;
	triangle->mat->vt->destroy(triangle->mat);
	rt_free(ent);
}

int
	triangle_compare(t_entity *ent, t_vec pos, t_vec dir)
{
	t_triangle	*triangle;
	FLOAT		f0;
	FLOAT		f1;
	FLOAT		f2;

	triangle = (t_triangle *) ent;
	f0 = vec_dot(dir, vec_sub(triangle->pos0, pos));
	f1 = vec_dot(dir, vec_sub(triangle->pos1, pos));
	f2 = vec_dot(dir, vec_sub(triangle->pos2, pos));
	if (f0 <= 0 && f1 <= 0 && f2 <= 0)
		return (-1);
	if (f0 >= 0 && f1 >= 0 && f2 >= 0)
		return (1);
	return (0);
}


int
	triangle_get_bounds(const t_entity *ent, t_vec *a, t_vec *b)
{
	t_triangle	*triangle;

	triangle = (t_triangle *) ent;
	*a = vec_min(vec_min(triangle->pos0, triangle->pos1), triangle->pos2);
	*b = vec_max(vec_min(triangle->pos0, triangle->pos1), triangle->pos2);
	return (1);
}

