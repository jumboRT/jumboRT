#include "scene.h"

#include <math.h>


#include <stdio.h>
#include <stdlib.h>

const t_entity_vt
	*cone_vt(void)
{
	static const t_entity_vt	vt = {
		cone_hit,
		cone_destroy
	};

	return (&vt);
}

/*
static t_ray
	cone_transform_ray(t_ray ray, t_cone *cone)
{
	FLOAT	z_rot;
	FLOAT	x_rot;

	ray.pos = vec_sub(ray.pos, cone->pos);
	if (fabs(cone->dir.v[Y]) > 0.0001)
	{
		z_rot = atan(cone->dir.v[X] / cone->dir.v[Y]);
		ray.dir = vec_rotate(vec_z(1), ray.dir, z_rot);
		ray.pos = vec_rotate(vec_z(1), ray.pos, z_rot);
	}
	if (fabs(cone->dir.v[Z]) > 0.0001)
	{
		x_rot = atan(cone->dir.v[Y] / cone->dir.v[Z]);
		ray.dir = vec_rotate(vec_x(1), ray.dir, x_rot);
		ray.pos = vec_rotate(vec_x(1), ray.pos, x_rot);
	}
	return (ray);
}
*/

int
	cone_hit(t_entity *ent, t_ray ray, t_hit *hit, FLOAT min)
{
	t_cone	*cone;
	FLOAT	a, b, c;
	FLOAT	d;
	t_vec	CO;
	FLOAT	costheta2;
	FLOAT	t1, t2, t_end;
	t_vec	rel_pos;
	FLOAT	divisor;
	FLOAT	r;
	
	cone = (t_cone *) ent;
	CO = vec_sub(ray.pos, cone->pos);
	costheta2 = pow(cos(cone->angle), 2.0);
	a = pow(vec_dot(ray.dir, cone->dir), 2.0) - costheta2;
	b = 2 * ((vec_dot(ray.dir, cone->dir) * vec_dot(CO, cone->dir)) - (vec_dot(ray.dir, CO) * costheta2));
	c = pow(vec_dot(CO, cone->dir), 2.0) - (vec_dot(CO, CO) * costheta2);
	d = (b * b) - (4 * a * c);
	if (d < 0)
		return (0);
	hit->mat = cone->mat;

	t1 = (-b + sqrt(d)) / (2 * a);
	t2 = (-b - sqrt(d)) / (2 * a);
	divisor = vec_dot(ray.dir, cone->dir);
	t_end = HUGE_VAL;
	if (!float_eq(divisor, 0, 0.001))
		t_end = vec_dot(vec_sub(vec_add(cone->pos, vec_scale(cone->dir, cone->height)), ray.pos), cone->dir) / divisor;
	
	hit->t = HUGE_VAL;
	if (t1 >= min)
	{
		hit->pos = vec_add(ray.pos, vec_scale(ray.dir, t1));
		rel_pos = vec_sub(hit->pos, cone->pos);
		if (vec_dot(rel_pos, cone->dir) >= 0)
		{
			if (vec_mag(vec_scale(cone->dir, vec_dot(rel_pos, cone->dir))) <= cone->height)
			{
				hit->t = t1;
				hit->normal = vec_norm(vec_cross(rel_pos, vec_cross(rel_pos, cone->dir)));
			}
		}
	}
	if (t2 < hit->t && t2 >= min)
	{

		hit->pos = vec_add(ray.pos, vec_scale(ray.dir, t2));
		rel_pos = vec_sub(hit->pos, cone->pos);
		if (vec_dot(rel_pos, cone->dir) >= 0)
		{
			if (vec_mag(vec_scale(cone->dir, vec_dot(rel_pos, cone->dir))) <= cone->height)
			{
				hit->t = t2;
				hit->normal = vec_norm(vec_cross(rel_pos, vec_cross(rel_pos, cone->dir)));
			}
		}
	}
	if (t_end < hit->t && t_end < t2 && t_end >= min)
	{
		r = tan(cone->angle) * cone->height;
		hit->pos = vec_add(ray.pos, vec_scale(ray.dir, t_end));
		rel_pos = vec_sub(hit->pos, cone->pos);
		if (cos(cone->angle) * vec_mag(rel_pos) <= r)
		{
			hit->t = t_end;
			hit->normal = cone->dir;
		}
	}
	if (hit->t == HUGE_VAL)
		return (0);
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
