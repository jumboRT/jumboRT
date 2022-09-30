#include "bsdf.h"
#include "world.h"

t_sample
	reflective_sample(t_trace_ctx *ctx, const t_world_hit *hit,
			const GLOBAL t_bxdf_reflective *bxdf, t_vec wiw)
{
	t_sample	result;
	t_vec		color;

	result.wo = wiw;
	result.wo = vec_set(result.wo, 0, -x(result.wo));
	result.wo = vec_set(result.wo, 1, -y(result.wo));
	color = filter_sample(ctx->world, bxdf->base.tex, hit->hit.uv);
	result.bsdf = vec_scale(color, 1.0f / rt_abs(z(result.wo)));
	result.pdf = 1;
	return (result);
}

t_vec
	reflective_f(t_trace_ctx *ctx, const t_world_hit *hit,
			const GLOBAL t_bxdf_reflective *bxdf, t_vec wiw, t_vec wow)
{
	(void) ctx;
	(void) hit;
	(void) bxdf;
	(void) wiw;
	(void) wow;
	return (vec_0());
}

float
	reflective_pdf(t_trace_ctx *ctx, const t_world_hit *hit,
			const GLOBAL t_bxdf_reflective *bxdf, t_vec wiw, t_vec wow)
{
	(void) ctx;
	(void) hit;
	(void) bxdf;
	(void) wiw;
	(void) wow;
	return (0);
}

