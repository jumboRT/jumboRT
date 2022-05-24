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
	t_vec	p, q;
	t_vec	d, m, n;
	FLOAT	r;
	FLOAT	md, nd, dd;
	FLOAT	nn, mn;
	FLOAT	a, k, c;
	FLOAT	b;
	FLOAT	discr;

	cyl = *(t_cylinder *) ent;
	p = cyl.pos;
	q = vec_add(cyl.pos, vec_scale(cyl.dir, cyl.height));
	r = cyl.diameter / 2;
	n = vec_scale(ray.dir, 50);
	m = vec_sub(ray.pos, cyl.pos);
	d = vec_sub(p, q);

	md = vec_dot(m, d);
	nd = vec_dot(n, d);
	dd = vec_dot(d, d);

	if (md < 0.0 && md + nd < 0.0)
		return (0);
	if (md > dd && md + nd > dd)
		return (0);
	nn = vec_dot(n, n);
	mn = vec_dot(m, n);
	a = dd * nn - nd * nd;
	k = vec_dot(m, m) - r * r;
	c = dd * k - md * md;
	hit->mat = cyl.mat;
	if (fabs(a) < 0.0000000001) {
		if (c > 0.0)
			return (0);
		if (md < 0.0)
			hit->t = -mn / nn;
		else if (md > dd)
			hit->t = (nd - mn) / nn;
		else
			hit->t = 0.0;
		hit->pos = vec_add(ray.pos, vec_scale(ray.dir, hit->t));
		hit->normal = vec(0.0, 0.0, 1.0, 0.0);
		return (1);
	}
	b = dd * mn - nd * md;
	discr = b * b - a * c;
	if (discr < 0.0)
		return (0);
	hit->t = (-b - sqrt(discr)) / a;
	hit->normal = vec(0.0, 0.0, 1.0, 0.0);
//	if (hit->t < 0.0 || hit->t > 1.0)
//		return (0);
	if (md + hit->t * nd < 0.0) {
		if (nd <= 0.0)
			return (0);
		hit->t = -md / nd;
		hit->pos = vec_add(ray.pos, vec_scale(ray.dir, hit->t));
//		return (k + 2 * hit->t * (mn + hit->t * nn) <= 0.0);
		return (1);
	} else if (md + hit->t * nd > dd) {
		if (nd >= 0.0)
			return (0);
		hit->t = (dd - md) / nd;
		hit->pos = vec_add(ray.pos, vec_scale(ray.dir, hit->t));
//		return (k + dd - 2 * md + hit->t * (2 * (mn - nd) + hit->t * nn) <= 0.0);
		return (1);
	}
	hit->pos = vec_add(ray.pos, vec_scale(ray.dir, hit->t));
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
