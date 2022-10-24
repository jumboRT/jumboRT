#include "bsdf.h"
#include "world.h"
#include "mat.h"

t_sample
	specular_sample(const t_bxdf_ctx *ctx,
			const GLOBAL t_bxdf_specular *bxdf)
{
	t_sample	result;
	t_vec		color;
	float		fresnel;

	result.wo = vec3(x(ctx->wi), y(ctx->wi), -z(ctx->wi));
	fresnel = f_dielectric(costheta(result.wo), 1.0f,
			ctx->hit->mat->refractive_index);
	color = vec_scale(filter_sample(ctx->ctx->world, bxdf->base.tex,
				ctx->hit->hit.uv), fresnel);
	result.bsdf = vec_0();
	result.pdf = 0.0f;
	if (vec_dot(ctx->gn, ctx->wi) * vec_dot(ctx->gn, result.wo) < 0)
	{
		result.bsdf = vec_scale(color, 1.0f / rt_abs(costheta(result.wo)));
		result.pdf = 1;
	}
	return (result);
}

t_vec
	specular_f(const t_bxdf_ctx *ctx,
			const GLOBAL t_bxdf_specular *bxdf, t_vec wo)
{
	(void) ctx;
	(void) bxdf;
	(void) wo;
	return (vec_0());
}

float
	specular_pdf(const t_bxdf_ctx *ctx,
			const GLOBAL t_bxdf_specular *bxdf, t_vec wo)
{
	(void) ctx;
	(void) bxdf;
	(void) wo;
	return (0.0f);
}
