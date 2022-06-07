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

/*
int
	triangle_hit(const t_entity *ent, t_ray ray, t_hit *hit, FLOAT min)
{
	t_triangle	*triangle;
	t_vec		normal;
	t_vec		norm1;
	t_vec		norm2;
	t_vec		norm3;
	FLOAT		divisor;
	FLOAT		dividend;
	FLOAT		t;
	t_vec		n; //TODO These values can be calculated right after parsing and won't change
	t_vec		m;
	t_vec		s;
	t_vec		e1;
	t_vec		e2;
	t_vec		tuv;

	triangle = (t_triangle *) ent;
	normal = vec_cross(vec_sub(triangle->pos1, triangle->pos2), vec_sub(triangle->pos2, triangle->pos0));
	divisor = vec_dot(ray.dir, normal);
	if (float_eq(divisor, 0, 0.001))
		return (0);
	dividend = vec_dot(vec_sub(triangle->pos1, ray.pos), normal);
	t = dividend / divisor;
	if (t < min)
		return (0);
	hit->normal = normal;
	hit->t = t;
	hit->mat = triangle->mat;
	hit->pos = vec_add(ray.pos, vec_scale(ray.dir, t));
	norm1 = vec_cross(vec_sub(hit->pos, triangle->pos2), vec_sub(triangle->pos2, triangle->pos0));
	norm2 = vec_cross(vec_sub(triangle->pos1, hit->pos), vec_sub(hit->pos, triangle->pos0));
	norm3 = vec_cross(vec_sub(triangle->pos1, triangle->pos2), vec_sub(triangle->pos2, hit->pos));
	if (vec_dot(norm1, norm2) < 0 || vec_dot(norm2, norm3) < 0)
		return (0);
	return (1);
	e1 = vec_sub(triangle->pos1, triangle->pos0);
	e2 = vec_sub(triangle->pos2, triangle->pos0);
	s = vec_sub(ray.pos, triangle->pos0);
	n = vec_cross(e1, e2);
	m = vec_cross(s, ray.dir);
	tuv = vec_scale(vec(
				vec_dot(n, s),
				vec_dot(m, e2),
				vec_dot(vec_neg(m), e1),
				0.0),
			1.0 / vec_dot(vec_neg(n), ray.dir));
	if (tuv.v[X] < 0 || tuv.v[Y] < 0 || tuv.v[Z] < 0)
		return (0);
	return (1);
}
*/

int
	triangler_hit(const t_entity *ent, t_ray ray, t_hit *hit, FLOAT min)
{
	const t_triangle	*triangle;
	FLOAT				t;
	t_vec				norm[3];
	t_vec				pos;

	triangle = (const t_triangle*) ent;
	if (!ray_plane_intersect(ray, triangle->pos0, triangle->normal, &t))
		return (0);
	if (t < min)
		return (0);
	pos = ray_at_t(ray, t);
	norm[0] = vec_cross(vec_sub(pos, triangle->pos2), vec_sub(triangle->pos2, triangle->pos0));
	norm[1] = vec_cross(vec_sub(triangle->pos1, pos), vec_sub(pos, triangle->pos0));
	norm[2] = vec_cross(vec_sub(triangle->pos1, triangle->pos2), vec_sub(triangle->pos2, pos));
	if (vec_dot(norm[0], norm[1]) < 0 || vec_dot(norm[1], norm[2]) < 0)
		return (0);
	hit->t = t;
	hit->mat = triangle->mat;
	hit->pos = ray_at_t(ray, t);
	hit->normal = triangle->normal;
	return (1);
}

/* https://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates */
int
	triangle_hit(const t_entity *ent, t_ray ray, t_hit *hit, FLOAT min)
{
	const t_triangle	*triangle;
	FLOAT				t;
	t_vec				pos;
	t_vec				v[3];
	FLOAT				uvw[3];
	FLOAT				d00, d01, d11, d20, d21;
	FLOAT				denom;

	triangle = (const t_triangle *) ent;
	if (!ray_plane_intersect(ray, triangle->pos0, triangle->normal, &t))
		return (0);
	if (t < min)
		return (0);
	pos = ray_at_t(ray, t);
	v[0] = vec_sub(triangle->pos1, triangle->pos0);
	v[1] = vec_sub(triangle->pos2, triangle->pos0);
	v[2] = vec_sub(pos, triangle->pos0);
	d00 = vec_dot(v[0], v[0]);
	d01 = vec_dot(v[0], v[1]);
	d11 = vec_dot(v[1], v[1]);
	d20 = vec_dot(v[2], v[0]);
	d21 = vec_dot(v[2], v[1]);
	/* This denom can be precomputed */
	denom = d00 * d11 - d01 * d01;
	uvw[1] = (d11 * d20 - d01 * d21) / denom;
	uvw[2] = (d00 * d21 - d01 * d20) / denom;
	uvw[0] = 1.0 - uvw[1] - uvw[2];
	if (uvw[0] < 0 || uvw[1] < 0 || uvw[2] < 0)
		return (0);
	if (!float_eq(uvw[0] + uvw[1] + uvw[2], 1.0, 0.001))
		return (0);
	hit->t = t;
	hit->pos = pos;
	hit->normal = triangle->normal;
	hit->mat = triangle->mat;
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

