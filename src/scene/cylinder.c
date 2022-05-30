#include "scene.h"

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
	z_rot = atan(cyl->dir.v[X] / cyl->dir.v[Y]);
	ray.dir = vec_rotate(vec_z(1), ray.dir, z_rot);
	ray.pos = vec_rotate(vec_z(1), ray.pos, z_rot);
	x_rot = atan(cyl->dir.v[Y] / cyl->dir.v[Z]);
	ray.dir = vec_rotate(vec_x(1), ray.dir, x_rot);
	ray.pos = vec_rotate(vec_x(1), ray.pos, x_rot);
	return (ray);
}

int
	cylinder_hit(t_entity *ent, t_ray ray, t_hit *hit, FLOAT min)
{
	t_cylinder	*cylinder;
	FLOAT		a;
	FLOAT		b;
	FLOAT		c;
	FLOAT		d;
	FLOAT		t1;
	FLOAT		t2;
	t_vec		v1;
	t_vec		v2;
	FLOAT		h;
	FLOAT		r;

	cylinder = (t_cylinder *) ent;
	hit->mat = cylinder->mat;
	r = cylinder->diameter / 2.0;
	ray = cylinder_transform_ray(ray, cylinder);
	if (float_eq(vec_dot(cylinder->dir, ray.dir), 1.0, 0.00001))
	{
		//Ray is parallel to cylinder
		if (vec_mag(ray.pos) > r)
			return (0);
	}
	a = pow(ray.dir.v[X], 2) + pow(ray.dir.v[X], 2);
	b = 2 * (ray.dir.v[X] * ray.pos.v[X] + ray.dir.v[Y] * ray.pos.v[Y]);
	c = pow(ray.pos.v[X], 2) + pow(ray.pos.v[Y], 2) - 1;
	d = (b * b) - (4 * a * c);
	if (d < 0)
		return (0);
	t1 = -b + sqrt(d) / (2 * a);
	t2 = -b - sqrt(d) / (2 * a);
	v1 = vec_add(ray.pos, vec_scale(ray.dir, t1));
	v2 = vec_add(ray.pos, vec_scale(ray.dir, t2));
	h = cylinder->height;
	if ((ray.pos.v[Z] > h && v1.v[Z] > h && v2.v[Z] < h)
			|| (ray.pos.v[Z] < h && v1.v[Z] < h && v2.v[Z] > h))
	{
		// hit top
	}
	if ((ray.pos.v[Z] > 0 && v1.v[Z] > 0 && v2.v[Z] < 0)
			|| (ray.pos.v[Z] < 0 && v1.v[Z] < 0 && v2.v[Z] > 0))
	{
		// hit bottom
	}
}

void
	cylinder_destroy(t_entity *ent)
{
	t_cylinder	*cylinder;

	cylinder = (t_cylinder *) ent;
	cylinder->mat->vt->destroy(cylinder->mat);
	rt_free(ent);
}
