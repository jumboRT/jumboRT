#include "bsdf.h"
#include "world.h"

t_sample
	diffuse_sample(const t_bxdf_ctx *ctx,
			const GLOBAL t_bxdf_diffuse *bxdf)
{
	t_sample	result;

	result.wo = rt_random_cosine_hemi(&ctx->ctx->ctx->seed);
	if (z(ctx->wi) > 0)
		result.wo = vec_set(result.wo, 2, -z(result.wo));
	result.bsdf = diffuse_f(ctx, bxdf, result.wo);
	result.pdf = diffuse_pdf(ctx, bxdf, result.wo);
	return (result);
}

t_vec
	diffuse_f(const t_bxdf_ctx *ctx,
			const GLOBAL t_bxdf_diffuse *bxdf, t_vec wow)
{
	t_vec	color;

	/*
	if (same_hemi(ctx->wi, wow))
		return (vec_0());
		*/
	color = filter_sample(ctx->ctx->world, bxdf->base.tex, ctx->hit->hit.uv);
	return (vec_scale(color, RT_1_PI));
}

float
	diffuse_pdf(const t_bxdf_ctx *ctx,
			const GLOBAL t_bxdf_diffuse *bxdf, t_vec wow)
{
	(void) ctx;
	(void) bxdf;
	if (same_hemi(ctx->wi, wow))
		return (0);
	return (rt_abs(z(wow)) * RT_1_PI);
}
