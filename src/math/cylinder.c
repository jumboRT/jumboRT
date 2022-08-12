#include "rtmath.h"

t_cylinder
	cylinder(t_vec pos, t_vec dir, FLOAT height, FLOAT radius)
{
	t_cylinder result;

	result.pos = pos;
	result.dir = dir;
	result.height = height;
	result.radius = radius;
	return (result);
}

static t_vec2
	cylinder_uv_mantle(t_cylinder cylinder, t_vec rel_point)
{
	FLOAT	u;
	FLOAT	v;

	u = rt_atan2(vec_dot(rel_point, vec_x(1.0)), vec_dot(rel_point, vec_y(1.0))) / RT_2PI;
	v = (vec_dot(rel_point, vec_z(1.0)) / cylinder.height) + 0.0;
	return vec2(u, (v / 2.0) + 0.5);
}

static t_vec2
	cylinder_uv_cap(t_cylinder cylinder, t_vec rel_point)
{
	t_vec2	uv;

	uv = vec2(x(rel_point), y(rel_point));
	uv = vec2_scale(uv, 1.0 / (4.0 * cylinder.radius));
	uv = vec2_add(uv, vec2(0.5, 0.5));
	uv = vec2_add(uv, vec2(0.5 * (z(rel_point) > (cylinder.height / 2.0)), 0.0));
	return (uv);
}

static int
	ray_cylinder_intersect_parallel(t_ray relative_ray, t_cylinder cylinder, FLOAT min, t_hit *hit)
{
	if (vec_mag2(relative_ray.org) > cylinder.radius * cylinder.radius)
		return (0);
	if (z(relative_ray.org) < 0.0 && z(relative_ray.org) >= min)
	{
		hit->geometric_normal = vec_neg(cylinder.dir);
		hit->shading_normal = hit->geometric_normal;
		hit->t = -z(relative_ray.org);
	}
	else if (cylinder.height - z(relative_ray.org) >= min)
	{
		hit->geometric_normal = cylinder.dir;
		hit->shading_normal = hit->geometric_normal;
		hit->t = cylinder.height - z(relative_ray.org);
	}
	else
		return (0);
	return (1);
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

static int
	ray_infinite_cylinder_intersect(t_ray relative_ray, FLOAT radius, FLOAT intersections[2])
{
	t_quadratic	quadratic;

	quadratic.a = rt_pow(x(relative_ray.dir), 2.0) + rt_pow(y(relative_ray.dir), 2.0);
	quadratic.b = 2.0 *
			(x(relative_ray.dir) * x(relative_ray.org) + y(relative_ray.dir) * y(relative_ray.org));
	quadratic.c = rt_pow(x(relative_ray.org), 2.0) + rt_pow(y(relative_ray.org), 2.0) - (radius * radius);
	if (quadratic_solve(&quadratic, intersections) == 0)
		return (0);
	return (1);
}

static int
	ray_cylinder_intersect_normal(t_ray relative_ray, t_cylinder cylinder, FLOAT min, t_hit *hit)
{
	FLOAT	t_side[2];
	FLOAT	z_side[2];
	FLOAT	t_end[2];
	FLOAT	height;
	t_vec	axis;
	FLOAT	angle;

	if (!ray_infinite_cylinder_intersect(relative_ray, cylinder.radius, t_side))
		return (0);
	height = cylinder.height;
	z_side[0] = z(ray_at(relative_ray, t_side[0]));
	z_side[1] = z(ray_at(relative_ray, t_side[1]));
	t_end[0] = RT_HUGE_VAL;
	t_end[1] = -RT_HUGE_VAL;
	if (z(relative_ray.dir) != 0.0)
	{
		t_end[0] = (height - z(relative_ray.org)) / z(relative_ray.dir);
		t_end[1] = (0.0 - z(relative_ray.org)) / z(relative_ray.dir);
	}
	hit->t = RT_HUGE_VAL;
	if (t_end[0] < hit->t && t_end[0] >= min && between(height, z_side[0], z_side[1]))
	{
		hit->t = t_end[0];
		hit->geometric_normal = cylinder.dir;
		hit->uv = cylinder_uv_cap(cylinder, ray_at(relative_ray, hit->t));
	}
	if (t_end[1] < hit->t && t_end[1] >= min && between(0.0, z_side[0], z_side[1]))
	{
		hit->t = t_end[1];
		hit->geometric_normal = vec_neg(cylinder.dir);
		hit->uv = cylinder_uv_cap(cylinder, ray_at(relative_ray, hit->t));
	}
	if (t_side[0] < hit->t && t_side[0] >= min && between(z_side[0], height, 0.0))
	{
		hit->t = t_side[0];
		vec_angles(vec_z(1), cylinder.dir, &axis, &angle);
		hit->geometric_normal = vec_scale(vec_set(ray_at(relative_ray, hit->t), 2, 0), 1 / cylinder.radius);
		hit->geometric_normal = vec_rotate(axis, hit->geometric_normal, angle);
		hit->uv = cylinder_uv_mantle(cylinder, ray_at(relative_ray, hit->t));
	}
	if (t_side[1] < hit->t && t_side[1] >= min && between(z_side[1], height, 0.0))
	{
		hit->t = t_side[1];
		vec_angles(vec_z(1), cylinder.dir, &axis, &angle);
		hit->geometric_normal = vec_scale(vec_set(ray_at(relative_ray, hit->t), 2, 0), 1 / cylinder.radius);
		hit->geometric_normal = vec_rotate(axis, hit->geometric_normal, angle);
		hit->uv = cylinder_uv_mantle(cylinder, ray_at(relative_ray, hit->t));
	}
	hit->shading_normal = hit->geometric_normal;
	if (hit->t < RT_HUGE_VAL)
		return (1);
	return (0);
}

void
	cylinder_hit_info(t_ray ray, t_cylinder cylinder, t_hit *hit)
{
	(void) ray;
	(void) cylinder;
	(void) hit;
}

int
	ray_cylinder_intersect(t_ray ray, t_cylinder cylinder, FLOAT min, t_hit *hit)
{
	t_ray	relative_ray;
	t_vec	axis;
	FLOAT	angle;

	vec_angles(vec_z(1), cylinder.dir, &axis, &angle);
	relative_ray.org = vec_rotate(axis, vec_sub(ray.org, cylinder.pos), -angle);
	relative_ray.dir = vec_rotate(axis, ray.dir, -angle);
	if (vec_dot(relative_ray.dir, cylinder.dir) == 1.0)
	{
		if (ray_cylinder_intersect_parallel(relative_ray, cylinder, min, hit))
		{
			hit->pos = ray_at(ray, hit->t);
			hit->dpdu = cylinder.dir;
			hit->dpdv = vec_norm(vec_cross(hit->dpdu, hit->geometric_normal));
			return (1);
		}
	}
	else
	{
		if (ray_cylinder_intersect_normal(relative_ray, cylinder, min, hit))
		{
			hit->pos = ray_at(ray, hit->t);
			hit->dpdu = cylinder.dir;
			if (vec_eq(cylinder.dir, vec_abs(hit->geometric_normal)))
				hit->dpdu = vec_tangent(cylinder.dir);
			hit->dpdv = vec_norm(vec_cross(hit->dpdu, hit->geometric_normal));
			return (1);
		}
	}
	return (0);
}
