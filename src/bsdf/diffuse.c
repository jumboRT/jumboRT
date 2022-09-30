#include "bsdf.h"
#include "world.h"

t_sample
	diffuse_sample(t_trace_ctx *ctx, const t_world_hit *hit,
			const GLOBAL t_bxdf_diffuse *bxdf, t_vec wiw)
{
	t_sample	result;

	result.wo = rt_random_cosine_hemi(&ctx->ctx->seed);
	if (z(wiw) > 0)
		result.wo = vec_neg(result.wo);
	result.bsdf = diffuse_f(ctx, hit, bxdf, wiw, result.wo);
	result.pdf = diffuse_pdf(ctx, hit, bxdf, wiw, result.wo);
	return (result);
}

t_vec
	diffuse_f(t_trace_ctx *ctx, const t_world_hit *hit,
			const GLOBAL t_bxdf_diffuse *bxdf, t_vec wiw, t_vec wow)
{
	t_vec	color;

	(void) wiw;
	(void) wow;
	color = filter_sample(ctx->world, bxdf->base.tex, hit->hit.uv);
	return (vec_scale(color, RT_1_PI));
}

float
	diffuse_pdf(t_trace_ctx *ctx, const t_world_hit *hit,
			const GLOBAL t_bxdf_diffuse *bxdf, t_vec wiw, t_vec wow)
{
	(void) ctx;
	(void) hit;
	(void) bxdf;
	if (same_hemi(wiw, vec_neg(wow)))
		return (rt_abs(z(wow)) * RT_1_PI);
	else
		return (0);
}

/*
t_sample
	diffuse_sample(t_trace_ctx *ctx, const t_world_hit *hit,
			const GLOBAL t_bxdf_diffuse *bxdf, t_vec wiw)
{
	t_sample	result;

	result.wo = rt_random_on_hemi(&ctx->ctx->seed, vec_z(1)); // TODO: same hemi
	result.bsdf = diffuse_f(ctx, hit, bxdf, wiw, result.wo);
	result.pdf = diffuse_pdf(ctx, hit, bxdf, wiw, result.wo);
	return (result);
}

t_vec
	diffuse_f(t_trace_ctx *ctx, const t_world_hit *hit,
			const GLOBAL t_bxdf_diffuse *bxdf, t_vec wiw, t_vec wow)
{
	t_vec	color;

	(void) wiw;
	(void) wow;
	color = filter_sample(ctx->world, bxdf->base.tex, hit->hit.uv);
	return (vec_scale(color, RT_1_PI));
}

float
	diffuse_pdf(t_trace_ctx *ctx, const t_world_hit *hit,
			const GLOBAL t_bxdf_diffuse *bxdf, t_vec wiw, t_vec wow)
{
	(void) ctx;
	(void) hit;
	(void) bxdf;
	if (same_hemi(wiw, vec_neg(wow)))
		return (1);
	else
		return (0);
}
*/

