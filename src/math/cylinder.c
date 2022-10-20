#include "rtmath.h"

static int
	cylinder_intersect_parallel(t_ray relative_ray, t_cylinder cylinder,
			float min, t_hit *hit)
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
	hit->dpdu = cylinder.dir;
	hit->dpdv = vec_norm(vec_cross(hit->dpdu, hit->geometric_normal));
	return (1);
}

static void
	cylinder_intersect_normal_part1(t_cylinder_ctx ctx)
{
	if (z(ctx.relative_ray.dir) != 0.0)
	{
		ctx.t_end[0]
			= (ctx.height - z(ctx.relative_ray.org)) / z(ctx.relative_ray.dir);
		ctx.t_end[1]
			= (0.0 - z(ctx.relative_ray.org)) / z(ctx.relative_ray.dir);
	}
	ctx.hit->t = RT_HUGE_VAL;
	if (ctx.t_end[0] < ctx.hit->t && ctx.t_end[0] >= ctx.min
		&& between(ctx.height, ctx.z_side[0], ctx.z_side[1]))
	{
		ctx.hit->t = ctx.t_end[0];
		ctx.hit->geometric_normal = ctx.cylinder.dir;
		ctx.hit->uv = cylinder_uv_cap(ctx.cylinder,
				ray_at(ctx.relative_ray, ctx.hit->t));
	}
	if (ctx.t_end[1] < ctx.hit->t && ctx.t_end[1] >= ctx.min
		&& between(0.0, ctx.z_side[0], ctx.z_side[1]))
	{
		ctx.hit->t = ctx.t_end[1];
		ctx.hit->geometric_normal = vec_neg(ctx.cylinder.dir);
		ctx.hit->uv = cylinder_uv_cap(ctx.cylinder,
				ray_at(ctx.relative_ray, ctx.hit->t));
	}
}

static void
	cylinder_intersect_normal_part2(t_cylinder_ctx ctx)
{
	if (ctx.t_side[0] < ctx.hit->t && ctx.t_side[0] >= ctx.min
		&& between(ctx.z_side[0], ctx.height, 0.0))
	{
		ctx.hit->t = ctx.t_side[0];
		vec_angles(vec_z(1), ctx.cylinder.dir, &ctx.axis, &ctx.angle);
		ctx.hit->geometric_normal = vec_scale(vec_set(
					ray_at(ctx.relative_ray, ctx.hit->t), 2, 0),
				1 / ctx.cylinder.radius);
		ctx.hit->geometric_normal
			= vec_rotate(ctx.axis, ctx.hit->geometric_normal, ctx.angle);
		ctx.hit->uv = cylinder_uv_mantle(ctx.cylinder,
				ray_at(ctx.relative_ray, ctx.hit->t));
	}
	if (ctx.t_side[1] < ctx.hit->t && ctx.t_side[1] >= ctx.min
		&& between(ctx.z_side[1], ctx.height, 0.0))
	{
		ctx.hit->t = ctx.t_side[1];
		vec_angles(vec_z(1), ctx.cylinder.dir, &ctx.axis, &ctx.angle);
		ctx.hit->geometric_normal = vec_scale(vec_set(ray_at(ctx.relative_ray,
						ctx.hit->t), 2, 0), 1 / ctx.cylinder.radius);
		ctx.hit->geometric_normal = vec_rotate(ctx.axis,
				ctx.hit->geometric_normal, ctx.angle);
		ctx.hit->uv = cylinder_uv_mantle(ctx.cylinder,
				ray_at(ctx.relative_ray, ctx.hit->t));
	}
}

static int
	cylinder_intersect_normal(t_ray relative_ray, t_cylinder cylinder,
			float min, t_hit *hit)
{
	t_cylinder_ctx	ctx;

	ctx.relative_ray = relative_ray;
	ctx.cylinder = cylinder;
	ctx.min = min;
	ctx.hit = hit;
	ctx.height = cylinder.height;
	if (!infinite_cylinder_intersect(ctx.relative_ray,
			ctx.cylinder.radius, ctx.t_side))
		return (0);
	ctx.z_side[0] = z(ray_at(ctx.relative_ray, ctx.t_side[0]));
	ctx.z_side[1] = z(ray_at(ctx.relative_ray, ctx.t_side[1]));
	ctx.t_end[0] = RT_HUGE_VAL;
	ctx.t_end[1] = -RT_HUGE_VAL;
	cylinder_intersect_normal_part1(ctx);
	cylinder_intersect_normal_part2(ctx);
	hit->shading_normal = hit->geometric_normal;
	hit->dpdu = cylinder.dir;
	if (vec_eq(vec_abs(cylinder.dir), vec_abs(hit->geometric_normal)))
		hit->dpdu = vec_tangent(cylinder.dir);
	hit->dpdv = vec_norm(vec_cross(hit->dpdu, hit->geometric_normal));
	if (hit->t < RT_HUGE_VAL)
		return (1);
	return (0);
}

int
	cylinder_intersect(t_ray ray, t_cylinder cylinder, float min,
			t_hit *hit)
{
	t_ray	relative_ray;
	t_vec	axis;
	float	angle;

	vec_angles(vec_z(1), cylinder.dir, &axis, &angle);
	relative_ray.org = vec_rotate(axis, vec_sub(ray.org, cylinder.pos), -angle);
	relative_ray.dir = vec_rotate(axis, ray.dir, -angle);
	if (vec_dot(relative_ray.dir, cylinder.dir) == 1.0)
	{
		if (cylinder_intersect_parallel(relative_ray, cylinder, min, hit))
		{
			hit->pos = ray_at(ray, hit->t);
			return (1);
		}
	}
	else
	{
		if (cylinder_intersect_normal(relative_ray, cylinder, min, hit))
		{
			hit->pos = ray_at(ray, hit->t);
			return (1);
		}
	}
	return (0);
}
