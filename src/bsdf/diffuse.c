#include "bsdf.h"
#include "world.h"

t_sample
	diffuse_sample(const t_bxdf_ctx *ctx,
			const GLOBAL t_bxdf_diffuse *bxdf)
{
	t_sample	result;

	result.wo = rt_random_cosine_hemi(&ctx->ctx->ctx->seed);
	if (vec_dot(ctx->gn, ctx->wi) * vec_dot(ctx->gn, result.wo) > 0)
		result.wo = vec_neg(result.wo);
	result.bsdf = diffuse_f(ctx, bxdf, result.wo);
	result.pdf = diffuse_pdf(ctx, bxdf, result.wo);
	return (result);
}

t_vec
	diffuse_f(const t_bxdf_ctx *ctx,
			const GLOBAL t_bxdf_diffuse *bxdf, t_vec wow)
{
	t_vec	color;

	if (vec_dot(ctx->gn, ctx->wi) * z(wow) > 0)
		return (vec_0());
	color = filter_sample(ctx->ctx->world, bxdf->base.tex, ctx->hit->hit.uv);
	return (vec_scale(color, RT_1_PI));
}

float
	diffuse_pdf(const t_bxdf_ctx *ctx,
			const GLOBAL t_bxdf_diffuse *bxdf, t_vec wow)
{
	(void) ctx;
	(void) bxdf;
	if (vec_dot(ctx->gn, ctx->wi) * z(wow) > 0)
		return (0);
	return (rt_abs(z(wow)) * RT_1_PI);
}
