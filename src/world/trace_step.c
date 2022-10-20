#include "world.h"
#include "bsdf.h"
#include "shape.h"

static int
	world_trace_debug_simple(t_trace_ctx *ctx, const t_world_hit *hit)
{
	float		uv_u;
	float		uv_v;

	if (ctx->world->render_mode == RT_RENDER_MODE_UV)
	{
		uv_u = rt_mod(rt_mod(u(hit->hit.uv), 1.0f) + 1.0f, 1.0f);
		uv_v = rt_mod(rt_mod(v(hit->hit.uv), 1.0f) + 1.0f, 1.0f);
		ctx->tail = vec(uv_u, uv_v, 0, 0);
		return (1);
	}
	if (ctx->world->render_mode == RT_RENDER_MODE_GEOMETRIC_NORMAL)
	{
		ctx->tail = vec_abs(hit->hit.geometric_normal);
		if (vec_dot(hit->hit.geometric_normal, ctx->ray.dir) > 0)
			ctx->tail = vec_scale(ctx->tail, 0.25);
		return (1);
	}
	if (ctx->world->render_mode == RT_RENDER_MODE_SHADING_NORMAL)
	{
		ctx->tail = vec_abs(hit->hit.shading_normal);
		if (vec_dot(hit->hit.shading_normal, ctx->ray.dir) > 0)
			ctx->tail = vec_scale(ctx->tail, 0.25);
		return (1);
	}
	return (0);
}

static int
	world_trace_debug(t_trace_ctx *ctx, const t_world_hit *hit)
{
	t_sample	sample;
	t_bxdf_ctx	bxdf_ctx;

	if (world_trace_debug_simple(ctx, hit))
		return (1);
	if (ctx->world->render_mode == RT_RENDER_MODE_ALBEDO)
	{
		if (hit->mat == 0)
		{
			ctx->tail = vec_0();
			return (1);
		}
		bxdf_ctx.ctx = ctx;
		bxdf_ctx.hit = hit;
		bxdf_ctx.wi = ctx->ray.dir;
		sample = bsdf_sample(&bxdf_ctx);
		ctx->tail = vec_abs(vec_scale(sample.bsdf, RT_PI));
		return (1);
	}
	return (0);
}

int
	world_trace_step_int(t_trace_ctx *ctx, t_world_hit *hit, t_sample *sample)
{
	t_bxdf_ctx	bxdf_ctx;

	ctx->alpha_skips = 0;
	if (RT_TRACE_LIGHT_SAMPLING)
		world_trace_light(ctx, hit);
	if (!hit->is_volume && (hit->is_ambient || ctx->specref
			|| prim_is_degenerate(hit->prim)))
		ctx->tail = vec_add(ctx->tail, vec_mul(ctx->head, le(ctx, hit)));
	bxdf_ctx.ctx = ctx;
	bxdf_ctx.hit = hit;
	bxdf_ctx.wi = ctx->ray.dir;
	*sample = bsdf_sample(&bxdf_ctx);
	if (sample->pdf == 0)
		return (0);
	ctx->head = vec_mul(ctx->head, vec_scale(sample->bsdf, rt_abs(vec_dot(
						sample->wo, hit->hit.shading_normal)) / sample->pdf));
	if (RT_TRACE_LIGHT_SAMPLING)
		ctx->specref = bxdf_is_perfspec(sample->bxdf);
	else
		ctx->specref = 1;
	return (1);
}

int
	world_trace_step(t_trace_ctx *ctx)
{
	t_world_hit	hit;
	t_sample	sample;

	intersect_full(ctx, &hit, ctx->ray, ctx->time);
	if (world_trace_debug(ctx, &hit) || hit.mat == 0)
		return (0);
	sample.wo = ctx->ray.dir;
	if (alpha_test(ctx, &hit))
	{
		if (!world_trace_step_int(ctx, &hit, &sample))
			return (0);
	}
	else
	{
		ctx->alpha_skips += 1;
		if (ctx->alpha_skips >= RT_TRACE_LIGHT_SLOW)
			ctx->specref = 1;
	}
	toggle_volume(ctx, &hit, sample.wo);
	ctx->ray.org = hit.hit.pos;
	ctx->ray.dir = sample.wo;
	ctx->time -= hit.hit.t;
	return (!vec_eq(ctx->head, vec_0()) && ctx->time > 0);
}
