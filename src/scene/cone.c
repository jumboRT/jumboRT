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
	FLOAT	t1, t2;
	t_vec	trans_pos;
	FLOAT	pos_height;
	
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
	
	hit->t = HUGE_VAL;
	if (t1 >= min)
		hit->t = t1;
	if (t2 < hit->t && t2 >= min)
		hit->t = t2;
	if (hit->t == HUGE_VAL)
		return (0);
	hit->pos = vec_add(ray.pos, vec_scale(ray.dir, hit->t));
	hit->normal = vec(1, 0, 0, 0);
	trans_pos = vec_sub(hit->pos, cone->pos);
	if (vec_dot(trans_pos, cone->dir) < 0)
		return (0);
	pos_height = vec_mag(vec_scale(cone->dir, vec_dot(trans_pos, cone->dir)));
	if (pos_height < 0 || pos_height > cone->height)
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
