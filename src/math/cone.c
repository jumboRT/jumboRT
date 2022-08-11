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

static t_vec2
	cone_uv_mantle(t_cone cone, FLOAT radius, t_vec point)
{
	t_vec op;

	op = vec_sub(point, cone.pos);
	return (vec2_add(vec2_scale(vec_change_basis2(op, vec_x(1.0), vec_y(1.0)), 1.0 / (4.0 * radius)), vec2(0.25, 0.25)));
}

static t_vec2
	cone_uv_cap(t_cone cone, FLOAT radius, t_vec point)
{
	t_vec	op;
	t_vec	tangent;
	t_vec2	uv;

	op = vec_sub(point, cone.pos);
	tangent = vec_norm(vec_tangent(cone.dir));
	uv = vec_change_basis2(op, tangent, vec_norm(vec_cross(tangent, cone.dir)));
	uv = vec2_scale(uv, 1.0 / (4.0 * radius));
	uv = vec2_add(uv, vec2(0.75, 0.25));
	return (uv);
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
	radius = rt_tan(cone.angle) * cone.height;
	if (end_hit.t < hit->t && end_hit.t >= min)
	{
		if (vec_mag2(vec_sub(end_hit.pos, vec_add(cone.pos, vec_scale(cone.dir, cone.height)))) <= radius * radius)
		{
			*hit = end_hit;
			hit->uv = cone_uv_cap(cone, radius, hit->pos);
		}
	}
	if (t_side[0] < hit->t && t_side[0] >= min)
	{
		hit->pos = ray_at(ray, t_side[0]);
		if (hit_on_finite_cone(hit->pos, cone))
		{
			hit->t = t_side[0];
			hit->geometric_normal = cone_normal_at(hit->pos, cone);
			hit->shading_normal = hit->geometric_normal;
			hit->dpdu = vec_norm(vec_sub(cone.pos, hit->pos));
			hit->dpdv = vec_norm(vec_cross(hit->dpdu, hit->geometric_normal));
			hit->uv = cone_uv_mantle(cone, radius, hit->pos);
		}
	}
	if (t_side[1] < hit->t && t_side[1] >= min)
	{
		hit->pos = ray_at(ray, t_side[1]);
		if (hit_on_finite_cone(hit->pos, cone))
		{
			hit->t = t_side[1];
			hit->geometric_normal = cone_normal_at(hit->pos, cone);
			hit->shading_normal = hit->geometric_normal;
			hit->dpdu = vec_norm(vec_sub(cone.pos, hit->pos));
			hit->dpdv = vec_norm(vec_cross(hit->dpdu, hit->geometric_normal));
			hit->uv = cone_uv_mantle(cone, radius, hit->pos);
		}
	}
	return (hit->t < RT_HUGE_VAL);
}
