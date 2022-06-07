#include "scene.h"

#include <math.h>


#include <stdio.h>
#include <stdlib.h>

const t_entity_vt
	*cone_vt(void)
{
	static const t_entity_vt	vt = {
		cone_hit,
		cone_destroy,
		cone_compare,
		cone_get_pos
	};

	return (&vt);
}

static int
	ray_cone_intersect(const t_cone *cone, t_ray ry, FLOAT intersects[2])
{
	t_quadratic	quadratic;
	FLOAT		costheta2;
	t_vec		co;

	co = vec_sub(ry.pos, cone->pos);
	costheta2 = cone->costheta2;
	quadratic.a = pow(vec_dot(ry.dir, cone->dir), 2.0) - costheta2;
	quadratic.b = 2 * ((vec_dot(ry.dir, cone->dir) * vec_dot(co, cone->dir)) - (vec_dot(ry.dir, co) * costheta2));
	quadratic.c = pow(vec_dot(co, cone->dir), 2.0) - (vec_dot(co, co) * costheta2);
	if (quadratic_solve(&quadratic, intersects) == 0)
		return (0);
	return (1);
}

int
	cone_hit(const t_entity *ent, t_ray ry, t_hit *hit, FLOAT min)
{
	const t_cone	*cone;
	FLOAT			t[2];
	FLOAT			t_end;
	FLOAT			dot;
	t_vec			relative_pos;

	cone = (const t_cone*) ent;
	if (!ray_cone_intersect(cone, ry, t))
		return (0);
	if (!ray_plane_intersect(cone->dir, ray_at_t(ray(cone->pos, cone->dir), cone->height), ry, &t_end))
		t_end = HUGE_VAL;
	hit->t = HUGE_VAL;
	if (t[0] >= min)
	{
		hit->pos = ray_at_t(ry, t[0]);
		relative_pos = vec_sub(hit->pos, cone->pos);
		dot = vec_dot(relative_pos, cone->dir);
		if (dot >= 0 && dot <= cone->height)
		{
			hit->t = t[0];
			hit->normal = vec_norm(vec_cross(relative_pos, vec_cross(relative_pos, cone->dir)));
		}
	}
	if (t[1] < hit->t && t[1] >= min)
	{
		hit->pos = ray_at_t(ry, t[1]);
		relative_pos = vec_sub(hit->pos, cone->pos);
		dot = vec_dot(vec_sub(hit->pos, cone->pos), cone->dir);
		if (dot >= 0 && dot <= cone->height)
		{
			hit->t = t[1];
			hit->normal = vec_norm(vec_cross(relative_pos, vec_cross(relative_pos, cone->dir)));
		}
	}
	if (t_end < hit->t && t_end >= min)
	{
		hit->pos = ray_at_t(ry, t_end);
		relative_pos = vec_sub(hit->pos, cone->pos);
		if (cone->costheta2 * vec_mag2(relative_pos) <= cone->r * cone->r)
		{
			hit->t = t_end;
			hit->normal = cone->dir;
		}
	}
	if (hit->t == HUGE_VAL)
		return (0);
	hit->mat = cone->mat;
	return (1);
}



void
	cone_destroy(t_entity *ent)
{
	t_cone	*cone;

	cone = (t_cone *) ent;
	cone->mat->vt->destroy(cone->mat);
	rt_free(ent);
}

int
	cone_compare(t_entity *ent, t_vec pos, t_vec dir)
{
	t_cone	*cone;
	FLOAT	radius;
	t_vec	pb;
	t_vec	ce;

	cone = (t_cone *) ent;
	radius = tan(cone->angle) * cone->height;
	pb = vec_add(cone->pos, vec_scale(cone->dir, cone->height));
	ce = vec(
		radius * sqrt(1 - pos.v[X] * pos.v[X]),
		radius * sqrt(1 - pos.v[Y] * pos.v[Y]),
		radius * sqrt(1 - pos.v[Z] * pos.v[Z]),
		0);
	return (box_plane_compare(pos, dir,
			vec_min(cone->pos, vec_sub(pb, ce)),
			vec_max(cone->pos, vec_add(pb, ce))));
}

t_vec
	cone_get_pos(const t_entity *ent)
{
	const t_cone	*cone;

	cone = (const t_cone *) ent;
	return (cone->pos);
}

