#include "../../include/scene.h"

#include <math.h>


#include <stdio.h>
#include <stdlib.h>

const t_entity_vt
	*cylinder_vt(void)
{
	static const t_entity_vt	vt = {
		cylinder_hit,
		cylinder_destroy,
		cylinder_compare,
		cylinder_get_pos
	};

	return (&vt);
}

static t_ray
	cylinder_transform_ray(t_ray ray, t_cylinder *cyl)
{
	FLOAT	z_rot;
	FLOAT	x_rot;

	ray.pos = vec_sub(ray.pos, cyl->pos);
	if (fabs(cyl->dir.v[Y]) > 0.0001)
	{
		z_rot = atan(cyl->dir.v[X] / cyl->dir.v[Y]);
		ray.dir = vec_rotate(vec_z(1), ray.dir, z_rot);
		ray.pos = vec_rotate(vec_z(1), ray.pos, z_rot);
	}
	if (fabs(cyl->dir.v[Z]) > 0.0001)
	{
		x_rot = atan(cyl->dir.v[Y] / cyl->dir.v[Z]);
		ray.dir = vec_rotate(vec_x(1), ray.dir, x_rot);
		ray.pos = vec_rotate(vec_x(1), ray.pos, x_rot);
	}
	return (ray);
}

static int
	between(FLOAT a, FLOAT b, FLOAT c)
{
	if (a < b && a > c)
		return (1);
	if (a > b && a < c)
		return (1);
	return (0);
}

int
	cylinder_hit(const t_entity *ent, t_ray ray, t_hit *hit, FLOAT min)
{
	t_cylinder	*cylinder;
	t_ray		org_ray;
	FLOAT		a;
	FLOAT		b;
	FLOAT		c;
	FLOAT		d;
	FLOAT		t1;
	FLOAT		t2;
	FLOAT		z1;
	FLOAT		z2;
	FLOAT		t_top;
	FLOAT		t_bot;
	FLOAT		h;
	FLOAT		r;

	cylinder = (t_cylinder *) ent;
	hit->mat = cylinder->mat;
	r = cylinder->diameter / 2.0;
	org_ray = ray;
	ray = cylinder_transform_ray(ray, cylinder);
	if (float_eq(vec_dot(cylinder->dir, ray.dir), 1.0, 0.00001))
	{
		//Ray is parallel to cylinder
		if (vec_mag(ray.pos) > r)
			return (0);
		if (ray.pos.v[Z] < 0 && ray.pos.v[Z] > min)
		{
			hit->normal = vec_neg(cylinder->dir);
			hit->t = -ray.pos.v[Z];
			hit->pos = vec_add(org_ray.pos, vec_scale(org_ray.dir, hit->t));
		}
		else
		{
			hit->normal = cylinder->dir;
			hit->t = cylinder->height - ray.pos.v[Z];
			hit->pos = vec_add(org_ray.pos, vec_scale(org_ray.dir, hit->t));
		}
		return (1);
	}
	a = pow(ray.dir.v[X], 2) + pow(ray.dir.v[Y], 2);
	b = 2 * (ray.dir.v[X] * ray.pos.v[X] + ray.dir.v[Y] * ray.pos.v[Y]);
	c = pow(ray.pos.v[X], 2) + pow(ray.pos.v[Y], 2) - pow(r, 2);
	d = (b * b) - (4 * a * c);
	if (d < 0)
		return (0);
	t1 = (-b - sqrt(d)) / (2 * a);
	t2 = (-b + sqrt(d)) / (2 * a);
	z1 = vec_add(ray.pos, vec_scale(ray.dir, t1)).v[Z];
	z2 = vec_add(ray.pos, vec_scale(ray.dir, t2)).v[Z];
	h = cylinder->height;
	t_top = HUGE_VAL;
	t_bot = -HUGE_VAL;
	if (ray.dir.v[Z] != 0)
	{
		t_top = (h - ray.pos.v[Z]) / ray.dir.v[Z];
		t_bot = (0 - ray.pos.v[Z]) / ray.dir.v[Z];
	}
	hit->t = HUGE_VAL;
	if (t_top < hit->t && t_top >= min && between(h, z1, z2))
	{
		hit->t = t_top;
		hit->pos = vec_add(org_ray.pos, vec_scale(org_ray.dir, hit->t));
		hit->normal = cylinder->dir;
	}
	if (t_bot < hit->t && t_bot >= min && between(0, z1, z2))
	{
		hit->t = t_bot;
		hit->pos = vec_add(org_ray.pos, vec_scale(org_ray.dir, hit->t));
		hit->normal = vec_neg(cylinder->dir);
	}
	if (t1 < hit->t && t1 >= min && between(z1, h, 0))
	{
		hit->t = t1;
		hit->pos = vec_add(org_ray.pos, vec_scale(org_ray.dir, hit->t));
		hit->normal = vec_norm(vec_sub(hit->pos, vec_add(cylinder->pos, vec_scale(cylinder->dir, vec_dot(vec_sub(hit->pos, cylinder->pos), cylinder->dir)))));
	}
	if (t2 < hit->t && t2 >= min && between(z2, h, 0))
	{
		hit->t = t2;
		hit->pos = vec_add(org_ray.pos, vec_scale(org_ray.dir, hit->t));
		hit->normal = vec_norm(vec_sub(hit->pos, vec_add(cylinder->pos, vec_scale(cylinder->dir, vec_dot(vec_sub(hit->pos, cylinder->pos), cylinder->dir)))));
	}
	if (hit->t != HUGE_VAL)
		return (1);
	return (0);
}

void
	cylinder_destroy(t_entity *ent)
{
	t_cylinder	*cylinder;

	cylinder = (t_cylinder *) ent;
	cylinder->mat->vt->destroy(cylinder->mat);
	rt_free(ent);
}

int
	cylinder_compare(t_entity *ent, t_vec pos, t_vec dir)
{
	t_cylinder	*cylinder;
	t_vec		pb;
	t_vec		ce;

	cylinder = (t_cylinder *) ent;
	pb = vec_add(cylinder->pos, vec_scale(cylinder->dir, cylinder->height));
	ce = vec(
		cylinder->diameter / 2 * sqrt(1 - pos.v[X] * pos.v[X]),
		cylinder->diameter / 2 * sqrt(1 - pos.v[Y] * pos.v[Y]),
		cylinder->diameter / 2 * sqrt(1 - pos.v[Z] * pos.v[Z]),
		0);
	return (box_plane_compare(pos, dir,
			vec_min(vec_sub(cylinder->pos, ce), vec_sub(pb, ce)),
			vec_max(vec_add(cylinder->pos, ce), vec_add(pb, ce))));
}

t_vec	cylinder_get_pos(const t_entity *ent)
{
	t_cylinder	*cylinder;

	cylinder = (t_cylinder *) ent;
	return (cylinder->pos);
}

