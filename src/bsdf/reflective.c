#include "bsdf.h"
#include "world.h"

t_sample
	reflective_sample(const t_bxdf_ctx *ctx,
			const GLOBAL t_bxdf_reflective *bxdf)
{
	t_sample	result;
	t_vec		color;

	result.wo = vec_set(ctx->wi, 2, -z(ctx->wi));
	color = filter_sample(ctx->ctx->world, bxdf->base.tex, ctx->hit->hit.uv);
	result.bsdf = vec_scale(color, 1.0f / rt_abs(z(result.wo)));
	result.pdf = 1;
	return (result);
}

t_vec
	reflective_f(const t_bxdf_ctx *ctx,
			const GLOBAL t_bxdf_reflective *bxdf, t_vec wow)
{
	(void) ctx;
	(void) bxdf;
	(void) wow;
	return (vec_0());
}

float
	reflective_pdf(const t_bxdf_ctx *ctx,
			const GLOBAL t_bxdf_reflective *bxdf, t_vec wow)
{
	(void) ctx;
	(void) bxdf;
	(void) wow;
	return (0);
}
