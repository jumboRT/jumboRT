#include "rtmath.h"

static int
	infinite_cone_intersect(t_ray ray, t_cone cone, float intersections[2])
{
	t_quadratic	quadratic;
	float		costheta2;
	t_vec		co;

	co = vec_sub(ray.org, cone.pos);
	costheta2 = rt_pow(rt_cos(cone.angle), 2.0);
	quadratic.a = rt_pow(vec_dot(ray.dir, cone.dir), 2.0) - costheta2;
	quadratic.b = 2 * ((vec_dot(ray.dir, cone.dir) * vec_dot(co, cone.dir))
			- (vec_dot(ray.dir, co) * costheta2));
	quadratic.c = rt_pow(vec_dot(co, cone.dir), 2.0)
		- (vec_dot(co, co) * costheta2);
	if (quadratic_solve(&quadratic, intersections) == 0)
		return (0);
	return (1);
}

void
	cone_hit_info(t_ray ray, t_cone cone, t_hit *hit)
{
	(void) ray;
	(void) cone;
	(void) hit;
}

static void
	mantle_hit_info(t_cone cone, t_hit *hit, float t)
{
	float	radius;

	radius = rt_tan(cone.angle) * cone.height;
	hit->t = t;
	hit->geometric_normal = cone_normal_at(hit->pos, cone);
	hit->shading_normal = hit->geometric_normal;
	hit->dpdu = vec_norm(vec_sub(cone.pos, hit->pos));
	hit->dpdv = vec_norm(vec_cross(hit->dpdu, hit->geometric_normal));
	hit->uv = cone_uv_mantle(cone, radius, hit->pos);
}

static void
	cone_intesect_part(t_hit end_hit, t_hit *hit, float min, t_cone cone)
{
	float	radius;

	radius = rt_tan(cone.angle) * cone.height;
	if (end_hit.t < hit->t && end_hit.t >= min)
	{
		if (vec_mag2(vec_sub(end_hit.pos,
					vec_add(cone.pos, vec_scale(cone.dir, cone.height))))
			<= radius * radius)
		{
			*hit = end_hit;
			hit->uv = cone_uv_cap(cone, radius, hit->pos);
		}
	}
}

int
	cone_intersect(t_ray ray, t_cone cone, float min, t_hit *hit)
{
	float	t_side[2];
	t_hit	end_hit;

	if (!infinite_cone_intersect(ray, cone, t_side))
		return (0);
	if (!plane_intersect(ray, plane(vec_add(cone.pos,
					vec_scale(cone.dir, cone.height)), cone.dir),
			min, &end_hit))
		end_hit.t = RT_HUGE_VAL;
	hit->t = RT_HUGE_VAL;
	cone_intesect_part(end_hit, hit, min, cone);
	if (t_side[0] < hit->t && t_side[0] >= min)
	{
		hit->pos = ray_at(ray, t_side[0]);
		if (hit_on_finite_cone(hit->pos, cone))
			mantle_hit_info(cone, hit, t_side[0]);
	}
	if (t_side[1] < hit->t && t_side[1] >= min)
	{
		hit->pos = ray_at(ray, t_side[1]);
		if (hit_on_finite_cone(hit->pos, cone))
			mantle_hit_info(cone, hit, t_side[1]);
	}
	return (hit->t < RT_HUGE_VAL);
}
