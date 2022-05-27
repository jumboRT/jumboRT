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

int
	cylinder_hit(t_entity *ent, t_ray ray, t_hit *hit, FLOAT min)
{
	t_cylinder	cyl;
	t_vec		cyl_base;
	t_vec		ray_base;
	FLOAT		radius;
	t_vec		cyl_dir;
	t_vec		ray_dir;
	t_vec		dist_vec;
	t_vec		ray_cyl;
	FLOAT		distance;
	FLOAT		t;
	FLOAT		s;
	t_vec		o;
	FLOAT		end_t1, end_t2;
	FLOAT		t_out, t_in;

	cyl = *(t_cylinder *) ent;
	cyl_base = cyl.pos;
	ray_base = ray.pos;
	radius = cyl.diameter / 2.0;
	cyl_dir = cyl.dir;
	ray_dir = ray.dir;
	hit->mat = cyl.mat;
	if (float_eq(vec_mag(vec_cross(ray_dir, cyl_dir)), 0.0, 0.00000001)) {
		/* TODO Implement paralel hit check*/
		return (0);
	}
	dist_vec = vec_scale(vec_cross(ray_dir, cyl_dir), 1.0 / vec_mag(vec_cross(ray_dir, cyl_dir)));
	ray_cyl = vec_sub(ray_base, cyl_base);
	distance = fabs(vec_dot(ray_cyl, dist_vec));
	if (distance > radius)
		return (0); /* Ray did not hit the cylinder */
	t = vec_dot(vec_cross(ray_cyl, cyl_dir), vec_cross(ray_dir, cyl_dir)) / pow(vec_mag(vec_cross(ray_dir, cyl_dir)), 2.0);
	o = vec_scale(vec_cross(dist_vec, cyl_dir), 1.0 / vec_mag(vec_cross(dist_vec, cyl_dir)));
	s = fabs(sqrt((radius * radius) - (distance * distance)) / vec_dot(ray_dir, o));
	t_out = t + s;
	t_in = t - s;
	//hit->t = t_in;
	end_t1 = - (vec_dot(ray_base, cyl_dir)) / (vec_dot(ray_dir, cyl_dir));
	end_t2 = - (vec_dot(ray_base, vec_neg(cyl_dir)) + cyl.height) / (vec_dot(ray_dir, vec_neg(cyl_dir)));
	rt_assert(ray_dir.v[W] == 0.0, "This should not happen1");
	rt_assert(cyl_dir.v[W] == 0.0, "This should not happen2");
	if (vec_dot(ray_dir, cyl_dir) < 0) {
		if (end_t1 < t_out) {
			t_out = end_t1;
		}
	} else if (vec_dot(ray_dir, cyl_dir) > 0) {
		if (end_t1 > t_in) {
			t_in = end_t1;
		}
	} else {
		return (0);
	}
	if (vec_dot(ray_dir, vec_neg(cyl_dir)) < 0) {
		if (end_t2 < t_out) {
			t_out = end_t2;
		}
	} else if (vec_dot(ray_dir, vec_neg(cyl_dir)) > 0) {
		if (end_t2 > t_in) {
			t_in = end_t2;
		}
	} else {
		return (0);
	}
	if (t_out < t_in)
		return (0); // TODO check with min ray len
	hit->t = t_in;
	hit->pos = vec_add(ray_base, vec_scale(ray_dir, hit->t));
	hit->normal = vec_sub(vec_norm(hit->pos), cyl_dir); /* TODO check if this normal calculation method works */
	return (1);
}

/*
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
//	d = vec_scale(cyl.dir, cyl.height);
	d = cyl.dir;
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
	hit->t = t2;
	if (t2 > t1)
		hit->t = t2;
	hit->pos = vec_add(ray.pos, vec_scale(ray.dir, hit->t));
	hit->normal = vec_sub(vec_norm(hit->pos), cyl.dir);
	if (vec_dot(vec_sub(hit->pos, cyl.pos), d) < 0)
		return (0);
	if (vec_dot(m, d) + (hit->t * vec_dot(n, d)) > vec_dot(d, d))
		return (0);
	return (1);
}
*/

void
	cylinder_destroy(t_entity *ent)
{
	t_cylinder	*cylinder;

	cylinder = (t_cylinder *) ent;
	cylinder->mat->vt->destroy(cylinder->mat);
	rt_free(ent);
}
