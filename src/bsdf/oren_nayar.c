#include "bsdf.h"
#include "rand.h"
#include "world.h"

t_sample
	oren_nayar_sample(const t_bxdf_ctx *ctx,
			const GLOBAL t_bxdf_oren_nayar *bxdf)
{
	t_sample	result;

	result.wo = rt_random_cosine_hemi(&ctx->ctx->ctx->seed);
	if (vec_dot(ctx->gn, ctx->wi) * vec_dot(ctx->gn, result.wo) > 0)
		result.wo = vec_neg(result.wo);
	result.bsdf = oren_nayar_f(ctx, bxdf, result.wo);
	result.pdf = oren_nayar_pdf(ctx, bxdf, result.wo);
	return (result);
}

static float
	oren_nayar_maxcos(t_vec wi, t_vec wo)
{
	float	sinphii;
	float	cosphii;
	float	sinphio;
	float	cosphio;
	float	dcos;

	sinphii = sinphi(wi);
	cosphii = cosphi(wi);
	sinphio = sinphi(wo);
	cosphio = cosphi(wo);
	dcos = cosphii * cosphio + sinphii * sinphio;
	return (rt_max(0, dcos));
}

t_vec
	oren_nayar_f(const t_bxdf_ctx *ctx,
			const GLOBAL t_bxdf_oren_nayar *bxdf, t_vec wo)
{
	t_vec	color;
	float	norminette[5];
	t_vec	wi;

	if (vec_dot(ctx->gn, ctx->wi) * z(wo) > 0)
		return (vec_0());
	wi = vec_neg(ctx->wi);
	norminette[0] = sintheta(wi);
	norminette[1] = sintheta(wo);
	norminette[2] = 0.0f;
	if (norminette[0] > 1e-4 && norminette[1] > 1e-4)
		norminette[2] = oren_nayar_maxcos(wi, wo);
	norminette[3] = norminette[0];
	norminette[4] = norminette[1] / abscostheta(wo);
	if (abscostheta(wi) > abscostheta(wo))
	{
		norminette[3] = norminette[1];
		norminette[4] = norminette[0] / abscostheta(wi);
	}
	color = filter_sample(ctx->ctx->world, bxdf->base.tex, ctx->hit->hit.uv);
	return (vec_scale(color, RT_1_PI * (
				bxdf->alpha + bxdf->beta * norminette[2] * norminette[3]
				* norminette[4])));
}

float
	oren_nayar_pdf(const t_bxdf_ctx *ctx,
			const GLOBAL t_bxdf_oren_nayar *bxdf, t_vec wo)
{
	(void) ctx;
	(void) bxdf;
	if (vec_dot(ctx->gn, ctx->wi) * z(wo) > 0)
		return (0);
	return (rt_abs(z(wo)) * RT_1_PI);
}
