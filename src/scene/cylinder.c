#include "scene.h"

#include <math.h>

const t_entity_vt
	*cylinder_vt(void)
{
	static const t_entity_vt	vt = {
		cylinder_hit,
		cylinder_destroy
	};

	return (&vt);
}

int
	cylinder_hit(t_entity *ent, t_ray ray, t_hit *hit)
{
	t_cylinder	cyl;
	t_vec		d;
	t_vec		n;
	t_vec		m;
	FLOAT		a, b, c;
	FLOAT		radius;
	FLOAT		discriminant;
	FLOAT		t1, t2;
	FLOAT		dividend, divisor;

	cyl = *(t_cylinder *) ent;
	hit->mat = cyl.mat;
	d = vec_scale(cyl.dir, cyl.height);
	n = ray.dir;
	radius = cyl.diameter / 2.0;
	m = vec_sub(ray.pos, cyl.pos);
	if (vec_dot(n, d) > 0)
	{
		dividend = -(vec_dot(m, d));
		divisor = vec_dot(n, d);
		if (!float_eq(divisor, 0, 0.001))
		{
			hit->t = dividend / divisor;
			hit->normal = vec_neg(cyl.dir);
			hit->pos = vec_add(ray.pos, vec_scale(ray.dir, hit->t));
			if (vec_dot(vec_sub(hit->pos, cyl.pos), vec_sub(hit->pos, cyl.pos)) <= radius * radius)
				return (1);
		}
	}
	else
	{
		dividend = vec_dot(d, d) - vec_dot(m, d);
		divisor = vec_dot(n, d);
		if (!float_eq(divisor, 0, 0.001))
		{
			hit->t = dividend / divisor;
			hit->normal = cyl.dir;
			hit->pos = vec_add(ray.pos, vec_scale(ray.dir, hit->t));
			if (vec_dot(vec_sub(hit->pos, vec_add(cyl.pos, vec_scale(cyl.dir, cyl.height))), vec_sub(hit->pos, vec_add(cyl.pos, vec_scale(cyl.dir, cyl.height)))) < radius * radius)
				return (1);
		}
	}
	a = vec_dot(vec_cross(d, n), vec_cross(d, n));
	b = vec_dot(vec_cross(d, m), vec_cross(d, n));
	c = vec_dot(vec_cross(d, m), vec_cross(d, m)) - (radius * radius * vec_dot(d, d));
	discriminant = (b * b) - a * c;
	if (discriminant < 0)
		return (0);
	t1 = (-b + sqrt(discriminant)) / a;
	t2 = (-b - sqrt(discriminant)) / a;
	hit->t = t1;
	if (t2 > t1)
		hit->t = t2;
	hit->pos = vec_add(ray.pos, vec_scale(ray.dir, hit->t));
	hit->normal = vec_sub(vec_norm(hit->pos), cyl.dir);
	if (vec_dot(vec_sub(hit->pos, cyl.pos), d) < 0)
		return (0);
	if (vec_dot(vec_sub(hit->pos, vec_add(cyl.pos, vec_scale(cyl.dir, cyl.height))), d) > vec_dot(d, d))
		return (0);
	return (1);
}

void
	cylinder_destroy(t_entity *ent)
{
	t_cylinder	*cylinder;

	cylinder = (t_cylinder *) ent;
	cylinder->mat->vt->destroy(cylinder->mat);
	rt_free(ent);
}
