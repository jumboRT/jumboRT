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
		cylinder_get_bounds
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
	if (relative_ray.dir.v[Z] != 0.0)
	{
		t_end[0] = (height - relative_ray.pos.v[Z]) / relative_ray.dir.v[Z];
		t_end[1] = (0.0 - relative_ray.pos.v[Z]) / relative_ray.dir.v[Z];
	}
	hit->t = HUGE_VAL;
	if (t_end[0] < hit->t && t_end[0] >= min && between(height, z[0], z[1]))
	{
		hit->t = t_end[0];
		hit->normal = cylinder->dir;
	}
	if (t_end[1] < hit->t && t_end[1] >= min && between(0.0, z[0], z[1]))
	{
		hit->t = t_end[1];
		hit->normal = vec_neg(cylinder->dir);
	}
	if (t_side[0] < hit->t && t_side[0] >= min && between(z[0], height, 0.0))
	{
		hit->t = t_side[0];
		hit->normal = cylinder_normal_at(cylinder->radius, ray_at_t(relative_ray, hit->t));
	}
	if (t_side[1] < hit->t && t_side[1] >= min && between(z[1], height, 0.0))
	{
		hit->t = t_side[1];
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
	hit->mat = cylinder->mat;
	if (float_eq(fabs(vec_dot(cylinder->dir, relative_ray.dir)), 1.0, 0.00001))
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
	t_vec	a;
	t_vec	b;

	cylinder_get_bounds(ent, &a, &b);
	return (box_plane_compare(pos, dir, a, b));
}

int
	cylinder_get_bounds(const t_entity *ent, t_vec *a, t_vec *b)
{
	t_cylinder	*cylinder;
	t_vec		pos;
	t_vec		pb;
	t_vec		ce;

	cylinder = (t_cylinder *) ent;
	pos = vec_scale(cylinder->dir, cylinder->height);
	pb = vec_add(cylinder->pos, pos);
	ce = vec(
		cylinder->radius * sqrt(1.0 - pos.v[X] * pos.v[X]),
		cylinder->radius * sqrt(1.0 - pos.v[Y] * pos.v[Y]),
		cylinder->radius * sqrt(1.0 - pos.v[Z] * pos.v[Z]),
		0);
	*a = vec_min(vec_sub(cylinder->pos, ce), vec_sub(pb, ce));
	*b = vec_max(vec_add(cylinder->pos, ce), vec_add(pb, ce));
	return (1);
}

