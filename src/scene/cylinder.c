#include "../../include/scene.h"

#include <math.h>


#include <stdio.h>
#include <stdlib.h>

const t_entity_vt
	*cylinder_vt(void)
{
	static const t_entity_vt	vt = {
		cylinder_hit,
		cylinder_destroy
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
	cylinder_hit(t_entity *ent, t_ray ray, t_hit *hit, FLOAT min)
{
	t_cylinder	*cylinder;
	t_ray		org_ray;
	FLOAT		a;
	FLOAT		b;
	FLOAT		c;
	FLOAT		d;
	FLOAT		t1;
	FLOAT		t2;
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
	h = cylinder->height;
	t_top = (h - ray.pos.v[Z]) / ray.dir.v[Z]; // TODO: division by 0
	t_bot = (0 - ray.pos.v[Z]) / ray.dir.v[Z]; // TODO: division by 0
	hit->t = HUGE_VAL; // TODO: try to get rid of this?
	if (t_top < hit->t && t_top >= min && between(t_top, t1, t2))
	{
		hit->t = t_top;
		hit->pos = vec_add(org_ray.pos, vec_scale(org_ray.dir, hit->t));
		hit->normal = cylinder->dir;
	}
	if (t_bot < hit->t && t_bot >= min && between(t_bot, t1, t2))
	{
		hit->t = t_bot;
		hit->pos = vec_add(org_ray.pos, vec_scale(org_ray.dir, hit->t));
		hit->normal = vec_neg(cylinder->dir);
	}
	if (t1 < hit->t && t1 >= min && between(t1, t_top, t_bot))
	{
		hit->t = t1;
		hit->pos = vec_add(org_ray.pos, vec_scale(org_ray.dir, hit->t));
		hit->normal = vec_norm(vec_sub(vec_scale(cylinder->dir, vec_dot(hit->pos, cylinder->dir)), hit->pos)); 
	}
	if (t2 < hit->t && t2 >= min && between(t2, t_top, t_bot))
	{
		hit->t = t2;
		hit->pos = vec_add(org_ray.pos, vec_scale(org_ray.dir, hit->t));
		hit->normal = vec_norm(vec_sub(vec_scale(cylinder->dir, vec_dot(hit->pos, cylinder->dir)), hit->pos)); 
	}
	if (hit->t != HUGE_VAL)
		return (1);
	return (0);
	/*
	if ((ray.pos.v[Z] > h && v1.v[Z] > h && v2.v[Z] < h)
			|| (ray.pos.v[Z] < h && v1.v[Z] < h && v2.v[Z] > h))
	{
		hit->normal = cylinder->dir;
		hit->t = vec_dot(ray.dir, vec_z(1)) / vec_dot(
				vec_sub(
						vec_scale(vec_z(1), cylinder->height), ray.pos), vec_z(1));
		hit->pos = vec_add(org_ray.pos, vec_scale(org_ray.dir, hit->t));
		if ((hit->t > t1 && hit->t < t2) || (hit->t > t2 && hit->t < t1))
			return (1);
		// hit top
	}
	if ((ray.pos.v[Z] > 0 && v1.v[Z] > 0 && v2.v[Z] < 0)
			|| (ray.pos.v[Z] < 0 && v1.v[Z] < 0 && v2.v[Z] > 0))
	{
		hit->normal = vec_neg(cylinder->dir);
		hit->t = vec_dot(ray.dir, vec_z(-1)) / vec_dot(vec_neg(ray.pos), vec_z(-1));
		hit->pos = vec_add(org_ray.pos, vec_scale(org_ray.dir, hit->t));
		if ((hit->t > t1 && hit->t < t2) || (hit->t > t2 && hit->t < t1))
			return (1);
		// hit bottom
	}
	hit->t = t1;
	if (t1 > t2 && t2 >= min)
		hit->t = t2;
	if (hit->t < min)
		return (0);
	hit->pos = vec_add(org_ray.pos, vec_scale(org_ray.dir, hit->t));
	if (hit->pos.v[Z] < 0 || hit->pos.v[Z] > cylinder->height)
		return (0);
	hit->normal = vec_norm(vec_sub(vec_norm(hit->pos), cylinder->dir));
	//hit->normal = vec_x(1);
	return (1);
	*/
}
//TODO check if it ever happens that the min distance of the ray completely skips an object

void
	cylinder_destroy(t_entity *ent)
{
	t_cylinder	*cylinder;

	cylinder = (t_cylinder *) ent;
	cylinder->mat->vt->destroy(cylinder->mat);
	rt_free(ent);
}
