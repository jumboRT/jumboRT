#include "bsdf.h"
#include "rand.h"
#include "world.h"

t_sample
	oren_nayar_sample(t_trace_ctx *ctx, const t_world_hit *hit,
			const GLOBAL t_bxdf_oren_nayar *bxdf, t_vec wi)
{
	t_sample	result;

	result.wo = rt_random_cosine_hemi(&ctx->ctx->seed);
	if (z(wi) > 0)
		result.wo = vec_set(result.wo, 2, -z(result.wo));
	result.bsdf = oren_nayar_f(ctx, hit, bxdf, wi, result.wo);
	result.pdf = oren_nayar_pdf(ctx, hit, bxdf, wi, result.wo);
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
	oren_nayar_f(t_trace_ctx *ctx, const t_world_hit *hit,
			const GLOBAL t_bxdf_oren_nayar *bxdf,
			t_vec wi, t_vec wo)
{
	t_vec	color;
	float	sinthetai;
	float	sinthetao;
	float	max_cos;
	float	sinalpha;
	float	tanbeta;

	wi = vec_neg(wi);
	sinthetai = sintheta(wi);
	sinthetao = sintheta(wo);
	max_cos = 0.0f;
	if (sinthetai > 1e-4 && sinthetao > 1e-4)
		max_cos = oren_nayar_maxcos(wi, wo);

	if (abscostheta(wi) > abscostheta(wo)) {
		sinalpha = sinthetao;
		tanbeta = sinthetai / abscostheta(wi);
	} else {
		sinalpha = sinthetai;
		tanbeta = sinthetao / abscostheta(wo);
	}
	color = filter_sample(ctx->world, bxdf->base.tex, hit->hit.uv);
	return (vec_scale(color, RT_1_PI * (
					bxdf->A + bxdf->B * max_cos * sinalpha
					* tanbeta)));
}

float
	oren_nayar_pdf(t_trace_ctx *ctx, const t_world_hit *hit,
			const GLOBAL t_bxdf_oren_nayar *bxdf,
			t_vec wi, t_vec wo)
{
	(void) ctx;
	(void) hit;
	(void) bxdf;
	if (same_hemi(wi, wo))
		return (0);
	return (rt_abs(z(wo)) * RT_1_PI);
}
