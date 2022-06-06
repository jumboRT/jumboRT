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
		triangle_get_pos
	};

	return (&vt);
}

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
	if (f0 < 0 && f1 < 0 && f2 < 0)
		return (-1);
	if (f0 > 0 && f1 > 0 && f2 > 0)
		return (1);
	return (0);
}


t_vec
	triangle_get_pos(const t_entity *ent)
{
	t_triangle	*triangle;

	triangle = (t_triangle *) ent;
	return (vec_scale(vec_add(vec_add(triangle->pos0, triangle->pos1), triangle->pos2), 1.0 / 3.0));
}

