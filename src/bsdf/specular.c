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

	result.wo = vec_set(wi, 2, -z(wi));
	fresnel = f_dielectric(rt_abs(costheta(result.wo)), ctx->refractive_index,
				hit->mat->refractive_index);
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
