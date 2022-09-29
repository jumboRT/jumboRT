#include "bsdf.h"
#include "world.h"

t_sample
	diffuse_sample(t_trace_ctx *ctx, const t_world_hit *hit,
			const GLOBAL t_bxdf_diffuse *bxdf, t_vec wiw)
{
	t_sample	result;
	float		epsilon0;
	float		epsilon1;
	float		theta;
	float		phi;

	epsilon0 = rt_random_float(&ctx->ctx->seed);
	epsilon1 = rt_random_float(&ctx->ctx->seed);
	theta = rt_acos(rt_sqrt(epsilon0));
	phi = RT_2PI * epsilon1;
	result.wo = sphere_to_cart(theta, phi);
	result.wo = rt_random_on_hemi_cos(&ctx->ctx->seed);
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
	if (vec_dot(wiw, wow) >= 0)
		return (rt_abs(z(wiw) * RT_1_PI));
	else
		return (0);
}
