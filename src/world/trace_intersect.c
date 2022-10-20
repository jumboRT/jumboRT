#include "world.h"
#include "shape.h"

void
	intersect_partial(const t_trace_ctx *ctx, t_world_hit *hit,
			t_ray ray, float max)
{
	hit->hit.t = max;
	hit->prim = 0;
	world_intersect(ctx->world, ray, hit);
	hit->mat = 0;
	hit->is_volume = 0;
	hit->is_ambient = 0;
	intersect_volume(ctx, hit);
	if (hit->hit.t >= max)
		hit->is_ambient = 1;
}

void
	intersect_info(const t_trace_ctx *ctx, t_world_hit *hit,
			t_ray ray)
{
	if (hit->is_volume)
	{
		hit->hit.uv = vec2(0, 0);
		hit->hit.pos = ray_at(ray, hit->hit.t);
		init_normals(hit, ray,
			rt_random_on_hemi(&ctx->ctx->seed, vec_neg(ray.dir)));
	}
	else if (hit->is_ambient)
	{
		hit->hit.pos = ray_at(ray, hit->hit.t);
		if (ctx->world->flags & RT_WORLD_HAS_AMBIENT)
			hit->mat = get_mat_const(ctx->world, ctx->world->ambient_mat);
		hit->hit.uv = sphere_uv_at(ray.dir);
		init_normals(hit, ray, ray.dir);
	}
	else
	{
		prim_hit_info(hit->prim, ctx->world, ray, hit);
		fix_normals(ctx->world, hit, ray);
	}
}

void
	intersect_full(const t_trace_ctx *ctx, t_world_hit *hit,
			t_ray ray, float max)
{
	intersect_partial(ctx, hit, ray, max);
	intersect_info(ctx, hit, ray);
}

void
	intersect_slow(const t_trace_ctx *ctx, t_world_hit *hit,
			t_ray ray, float max)
{
	t_trace_ctx	ctx2;
	int			depth;
	float		t;

	ctx2 = *ctx;
	ctx2.ray = ray;
	depth = 0;
	t = 0;
	while (depth < RT_TRACE_LIGHT_SLOW)
	{
		intersect_partial(&ctx2, hit, ray, max - t);
		if (hit->is_ambient)
			t = max;
		if (hit->is_ambient || hit->is_volume)
			break ;
		prim_hit_info(hit->prim, ctx->world, ray, hit);
		fix_normals(ctx->world, hit, ray);
		t += hit->hit.t;
		if (alpha_test(&ctx2, hit))
			break ;
		toggle_volume(&ctx2, hit, ray.dir);
		ray.org = hit->hit.pos;
		depth += 1;
	}
	hit->hit.t = t;
}
