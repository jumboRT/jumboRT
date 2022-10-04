#include "bsdf.h"
#include "world.h"

t_sample
	phong_sample(t_trace_ctx *ctx, const t_world_hit *hit,
			const GLOBAL t_bxdf_phong *bxdf, t_vec wiw)
{
	t_sample	result;

	result.wo = rt_random_cosine_hemi(&ctx->ctx->seed);
	if (z(wiw) > 0)
		result.wo = vec_neg(result.wo);
	result.bsdf = phong_f(ctx, hit, bxdf, wiw, result.wo);
	result.pdf = phong_pdf(ctx, hit, bxdf, wiw, result.wo);
	return (result);
}

t_vec
	phong_f(t_trace_ctx *ctx, const t_world_hit *hit,
			const GLOBAL t_bxdf_phong *bxdf, t_vec wiw, t_vec wow)
{
	t_vec	color;

	(void) wiw;
	(void) wow;
	color = filter_sample(ctx->world, bxdf->base.tex, hit->hit.uv);
	return (vec_scale(color, RT_1_PI));
}

float
	phong_pdf(t_trace_ctx *ctx, const t_world_hit *hit,
			const GLOBAL t_bxdf_phong *bxdf, t_vec wiw, t_vec wow)
{
	(void) ctx;
	(void) hit;
	(void) bxdf;
	if (same_hemi(wiw, vec_neg(wow)))
		return (rt_abs(z(wow)) * RT_1_PI);
	else
		return (0);
}

