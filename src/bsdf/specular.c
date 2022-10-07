#include "bsdf.h"
#include "world.h"
#include "mat.h"

t_sample
	specular_sample(t_trace_ctx *ctx, const t_world_hit *hit,
			const GLOBAL t_bxdf_specular *bxdf, t_vec wi)
{
	t_sample	result;
	t_vec		color;
	float		fresnel;
	float		etai;
	float		etat;

	/* TODO this check can be removed since f_dielectric also does it */
	wi = vec_neg(wi);

	etai = 1.0f;
	etat = hit->mat->refractive_index;
	/*
	if (costheta(wi) < 0)
	{
		etai = hit->mat->refractive_index;
		etat = 1.0f;
	}

	result.wo = vec_set(wi, 2, -z(wi));
	*/
	result.wo = vec3(-x(wi), -y(wi), z(wi));
	fresnel = f_dielectric(costheta(result.wo), etai, etat);
	color = vec_scale(filter_sample(ctx->world, bxdf->base.tex, hit->hit.uv),
				fresnel);
	result.bsdf = vec_scale(color, 1.0f / rt_abs(costheta(result.wo)));
	result.pdf = 1;
	return (result);
}

t_vec
	specular_f(t_trace_ctx *ctx, const t_world_hit *hit,
			const GLOBAL t_bxdf_specular *bxdf, t_vec wi, t_vec wo)
{
	(void) ctx;
	(void) hit;
	(void) bxdf;
	(void) wi;
	(void) wo;
	return (vec_0());
}

float
	specular_pdf(t_trace_ctx *ctx, const t_world_hit *hit,
			const GLOBAL t_bxdf_specular *bxdf, t_vec wi, t_vec wo)
{
	(void) ctx;
	(void) hit;
	(void) bxdf;
	(void) wi;
	(void) wo;
	return (0.0f);
}
