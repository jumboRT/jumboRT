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
	cylinder_transform_ray(t_ray ray, const t_cylinder *cyl)
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

/*
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
*/

static t_vec
	cylinder_normal_at(FLOAT radius, t_vec relative_point)
{
	/* I think we can do vec_scale(..., 1.0 / radius) instead of vec_norm */
	(void) radius;
	return (vec_norm(
				vec_sub(
					relative_point,
					vec_scale(vec_z(1.0), vec_dot(relative_point, vec_z(1.0))))));
}

static int
	cylinder_hit_parallel(const t_cylinder *cylinder, t_ray relative_ray, t_hit *hit, FLOAT min)
{
	if (vec_mag2(relative_ray.pos) > (cylinder->radius * cylinder->radius))
	{
		return (0);
	}
	if (relative_ray.pos.v[Z] < 0 && relative_ray.pos.v[Z] >= min)
	{
		hit->normal = vec_neg(cylinder->dir);
		hit->t = -relative_ray.pos.v[Z];
	}
	else if (cylinder->height - relative_ray.pos.v[Z] >= min)
	{
		hit->normal = cylinder->dir;
		hit->t = cylinder->height - relative_ray.pos.v[Z];
	}
	else
	{
		return (0);
	}
	return (1);
}

static int
	ray_cylinder_intersection(FLOAT radius, t_ray relative_ray, FLOAT intersections[2])
{
	t_quadratic	quadratic;

	quadratic.a = pow(relative_ray.dir.v[X], 2.0) + pow(relative_ray.dir.v[Y], 2.0);
	quadratic.b = 2.0 *
			(relative_ray.dir.v[X] * relative_ray.pos.v[X] + relative_ray.dir.v[Y] * relative_ray.pos.v[Y]);
	quadratic.c = pow(relative_ray.pos.v[X], 2.0) + pow(relative_ray.pos.v[Y], 2.0) - (radius * radius);
	if (quadratic_solve(&quadratic, intersections) == 0)
		return (0);
	return (1);
}

static int
	cylinder_hit_normal(const t_cylinder *cylinder, t_ray relative_ray, t_hit *hit, FLOAT min)
{
	FLOAT	t_side[2];
	FLOAT	z[2];
	FLOAT	t_end[2];
	FLOAT	height;

	if (!ray_cylinder_intersection(cylinder->radius, relative_ray, t_side))
		return (0);
	height = cylinder->height;
	z[0] = ray_at_t(relative_ray, t_side[0]).v[Z];
	z[1] = ray_at_t(relative_ray, t_side[1]).v[Z];
	t_end[0] = HUGE_VAL;
	t_end[1] = -HUGE_VAL;
	hit->t = HUGE_VAL;
	if (relative_ray.dir.v[Z] != 0)
	{
		t_end[0] = (height - relative_ray.pos.v[Z]) / relative_ray.dir.v[Z];
		t_end[1] = (0 - relative_ray.pos.v[Z]) / relative_ray.dir.v[Z];
	}
	hit->t = HUGE_VAL;
	if (t_end[0] < hit->t && t_end[0] >= min && between(height, z[0], z[1]))
	{
		hit->t = t_end[0];
		hit->normal = cylinder->dir;
	}
	if (t_end[1] < hit->t && t_end[1] >= min && between(0.0, z[0], z[1]))
	{
		hit->t = t_end[0];
		hit->normal = vec_neg(cylinder->dir);
	}
	if (t_side[0] < hit->t && t_side[0] >= min && between(z[0], height, 0.0))
	{
		hit->t = t_side[0];
		hit->normal = cylinder_normal_at(cylinder->radius, ray_at_t(relative_ray, hit->t));
	}
	if (t_side[1] < hit->t && t_side[1] >= min && between(z[1], height, 0.0))
	{
		hit->t = t_side [0];
		hit->normal = cylinder_normal_at(cylinder->radius, ray_at_t(relative_ray, hit->t));
	}
	if (hit->t == HUGE_VAL)
		return (0);
	hit->mat = cylinder->mat;
	return (1);
}

int
	cylinder_hit(const t_entity *ent, t_ray ray, t_hit *hit, FLOAT min)
{
	const t_cylinder	*cylinder;
	t_ray				relative_ray;
	
	cylinder = (const t_cylinder *) ent;
	relative_ray = cylinder_transform_ray(ray, cylinder);
	/* NOTE I think we need to check the absolute alue of the dot_product here */
	if (float_eq(vec_dot(cylinder->dir, relative_ray.dir), 1.0, 0.00001))
	{
		if (cylinder_hit_parallel(cylinder, relative_ray, hit, min))
		{
			hit->pos = ray_at_t(ray, hit->t);
			return (1);
		}
		return (0);
	}
	if (cylinder_hit_normal(cylinder, relative_ray, hit, min))
	{
		hit->pos = ray_at_t(ray, hit->t);
		return (1);
	}
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

