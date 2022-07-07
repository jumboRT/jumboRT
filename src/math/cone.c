#include "rtmath.h"

static int
	ray_infinite_cone_intersect(t_ray ray, t_cone cone, FLOAT intersections[2])
{
	t_quadratic	quadratic;
	FLOAT		costheta2;
	t_vec		co;

	co = vec_sub(ray.org, cone.pos);
	costheta2 = rt_pow(rt_cos(cone.angle), 2.0);
	quadratic.a = rt_pow(vec_dot(ray.dir, cone.dir), 2.0) - costheta2;
	quadratic.b = 2 * ((vec_dot(ray.dir, cone.dir) * vec_dot(co, cone.dir)) - (vec_dot(ray.dir, co) * costheta2));
	quadratic.c = rt_pow(vec_dot(co, cone.dir), 2.0) - (vec_dot(co, co) * costheta2);
	if (quadratic_solve(&quadratic, intersections) == 0)
		return (0);
	return (1);
}

static int
	hit_on_finite_cone(t_vec hit, t_cone cone)
{
	FLOAT	dot;

	dot = vec_dot(vec_sub(hit, cone.pos), cone.dir);
	return (dot >= 0 && dot <= cone.height);
}

static t_vec
	cone_normal_at(t_vec hit, t_cone cone)
{
	t_vec	relative_hit;

	relative_hit = vec_sub(hit, cone.pos);
	return (vec_norm(vec_cross(relative_hit, vec_cross(relative_hit, cone.dir))));
}

int
	ray_cone_intersect(t_ray ray, t_cone cone, FLOAT min, t_hit *hit)
{
	FLOAT	t_side[2];
	t_hit	end_hit;
	FLOAT	radius;

	if (!ray_infinite_cone_intersect(ray, cone, t_side))
		return (0);
	if (!ray_plane_intersect(ray, plane(vec_add(cone.pos, vec_scale(cone.dir, cone.height)), cone.dir), min, &end_hit))
		end_hit.t = RT_HUGE_VAL;
	hit->t = RT_HUGE_VAL;
	if (end_hit.t < hit->t && end_hit.t >= min)
	{
		radius = rt_tan(cone.angle) * cone.height;
		if (vec_mag2(vec_sub(end_hit.pos, vec_add(cone.pos, vec_scale(cone.dir, cone.height)))) <= radius * radius)
			*hit = end_hit;
	}
	if (t_side[0] < hit->t && t_side[0] >= min)
	{
		hit->pos = ray_at(ray, t_side[0]);
		if (hit_on_finite_cone(hit->pos, cone))
		{
			hit->t = t_side[0];
			hit->normal = vec_z(1);
			if (end_hit.t < RT_HUGE_VAL)
				hit->normal = vec_x(1);
			hit->normal = cone_normal_at(hit->pos, cone);
		}
	}
	if (t_side[1] < hit->t && t_side[1] >= min)
	{
		hit->pos = ray_at(ray, t_side[1]);
		if (hit_on_finite_cone(hit->pos, cone))
		{
			hit->t = t_side[1];
			hit->normal = vec_z(1);
			if (end_hit.t < RT_HUGE_VAL)
				hit->normal = vec_x(1);
			hit->normal = cone_normal_at(hit->pos, cone);
		}
	}
	return (hit->t < RT_HUGE_VAL);
}
