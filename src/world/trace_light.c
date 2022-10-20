#include "world.h"
#include "shape.h"
#include "mat.h"

static t_ray
	intersect_light_init(const t_trace_ctx *ctx, t_world_hit *hit,
			const t_world_hit *whit)
{
	uint32_t	light;
	t_vec		delta;

	light = rt_random(&ctx->ctx->seed) % ctx->world->lights_count;
	hit->prim = get_prim_const(ctx->world, ctx->world->lights[light]);
	hit->mat = get_mat_const(ctx->world, prim_mat(hit->prim));
	hit->hit.pos = prim_sample(hit->prim, ctx->world, ctx->ctx);
	delta = vec_sub(hit->hit.pos, whit->hit.pos);
	hit->hit.t = vec_mag(delta);
	return (ray(whit->hit.pos, vec_norm(delta)));
}

static int
	intersect_light_point(const t_trace_ctx *ctx, t_world_hit *hit,
			const t_world_hit *lhit, t_ray lray)
{
	if (hit->hit.t > ctx->time || lhit->hit.t < hit->hit.t)
		return (0);
	hit->hit.uv = vec2(0, 0);
	init_normals(hit, lray, lray.dir);
	return (1);
}

static int
	intersect_light(const t_trace_ctx *ctx, t_world_hit *hit,
			const t_world_hit *whit)
{
	t_world_hit	lhit;
	t_ray		lray;

	if (ctx->world->lights_count == 0)
		return (0);
	lray = intersect_light_init(ctx, hit, whit);
	if (!RT_TRACE_LIGHT_SLOW)
		intersect_partial(ctx, &lhit, lray, ctx->time);
	else
		intersect_slow(ctx, &lhit, lray, ctx->time);
	if (prim_type(hit->prim) == RT_SHAPE_POINT)
		return (intersect_light_point(ctx, hit, &lhit, lray));
	if (lhit.hit.t >= ctx->time || lhit.is_volume || lhit.prim != hit->prim)
		return (0);
	if (vec_mag(vec_sub(hit->hit.pos, lhit.hit.pos)) > RT_TINY_VAL)
		return (0);
	if (!RT_TRACE_LIGHT_SLOW)
	{
		prim_hit_info(hit->prim, ctx->world, lray, &lhit);
		fix_normals(ctx->world, &lhit, lray);
	}
	*hit = lhit;
	return (1);
}

t_vec
	le(t_trace_ctx *ctx, const t_world_hit *hit)
{
	t_vec	sample;
	float	brightness;

	if (!(hit->mat->flags & RT_MAT_EMITTER))
		return (vec_0());
	sample = filter_sample(ctx->world, hit->mat->emission, hit->hit.uv);
	brightness = hit->mat->brightness * rt_pow(rt_abs(vec_dot(ctx->ray.dir,
					hit->hit.shading_normal)), hit->mat->emission_exp);
	return (vec_scale(sample, brightness));
}

void
	world_trace_light(t_trace_ctx *ctx, const t_world_hit *hit)
{
	t_world_hit	lhit;
	t_sample	sample;
	t_vec		tmp;
	t_bxdf_ctx	bxdf_ctx;

	if (!intersect_light(ctx, &lhit, hit))
		return ;
	sample.wo = vec_norm(vec_sub(lhit.hit.pos, hit->hit.pos));
	bxdf_ctx.ctx = ctx;
	bxdf_ctx.hit = hit;
	bxdf_ctx.wi = ctx->ray.dir;
	sample.bsdf = bsdf_f(&bxdf_ctx, sample.wo);
	sample.pdf = rt_abs(vec_dot(sample.wo, lhit.hit.shading_normal));
	sample.pdf *= rt_abs(vec_dot(sample.wo, hit->hit.shading_normal));
	sample.pdf *= prim_area(lhit.prim, ctx->world);
	sample.pdf *= ctx->world->lights_count;
	sample.pdf /= rt_max(lhit.hit.t * lhit.hit.t, RT_TINY_VAL);
	tmp = vec_scale(vec_mul(le(ctx, &lhit), sample.bsdf), sample.pdf);
	ctx->tail = vec_add(ctx->tail, vec_mul(ctx->head, tmp));
}
