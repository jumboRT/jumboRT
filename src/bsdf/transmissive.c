#include "bsdf.h"

#include "world.h"
#include "mat.h"

static int
	refract(t_vec wi, t_vec n, float eta, t_vec *wt)
{
	float	costhetai;
	float	costhetat;
	float	sin2thetai;
	float	sin2thetat;

	costhetai = vec_dot(wi, n);
	sin2thetai = rt_max(0.0f, 1.0f - costhetai * costhetai);
	sin2thetat = eta * eta * sin2thetai;
	if (sin2thetat >= 1)
		return (0);
	costhetat = rt_sqrt(1.0f - sin2thetat);
	*wt = vec_add(vec_scale(vec_neg(wi), eta),
			vec_z(eta * costhetai - costhetat));
	return (1);
}

t_sample
	transmissive_sample(t_trace_ctx *ctx, const t_world_hit *hit,
			const GLOBAL t_bxdf_transmissive *bxdf, t_vec wi)
{
	t_vec		wiw;
	float		etai;
	float		etat;
	float		fresnel;
	t_sample	result;
	t_vec		n;

	wiw = local_to_world(hit, wi);
	wi = vec_neg(wi);

	etai = ctx->refractive_index;
	etat = hit->mat->refractive_index;
	n = vec_z(-1.0f);
	result.pdf = 0.0f;
	result.bsdf = vec_0();
	result.wo = vec3(-x(wi), -y(wi), z(wi));
	if (vec_dot(hit->hit.geometric_normal, wiw) > 0)
	{
		n = vec_neg(n);
		etai = hit->mat->refractive_index;
		etat = ctx->refractive_index;
	}
	if (!refract(wi, n, etai / etat, &result.wo))
	{
		return (result);
/* TODO Remove code below for reflection */
		result.wo = vec3(-x(wi), -y(wi), z(wi));
		result.bsdf = filter_sample(ctx->world, bxdf->base.tex, hit->hit.uv);
		fresnel = f_dielectric(costheta(result.wo), etai, etat);
		result.bsdf = vec_scale(result.bsdf, fresnel / rt_abs(vec_dot(n, wi)));
		return (result);
	}
	result.pdf = 1.0f;
	fresnel = f_dielectric(rt_abs(vec_dot(n, result.wo)), etai, etat);
	result.bsdf = vec_mul(filter_sample(ctx->world, bxdf->refraction_tex,
					hit->hit.uv), vec_sub(vec3(1.0f, 1.0f, 1.0f), 
					vec3(fresnel, fresnel, fresnel)));
	result.bsdf = vec_scale(result.bsdf, 1.0f / rt_abs(vec_dot(n, wi)));
	if (vec_dot(hit->hit.geometric_normal, wiw) > 0)
		ctx->refractive_index /= hit->mat->refractive_index;
	else
		ctx->refractive_index *= hit->mat->refractive_index;
	return (result);
}
/*
t_sample
	transmissive_sample(t_trace_ctx *ctx, const t_world_hit *hit,
			const GLOBAL t_bxdf_transmissive *bxdf, t_vec wi)
{
	t_sample	result;
	float		etai;
	float		etat;
	float		fresnel;
	t_vec		wiw;

	wiw = local_to_world(hit, wi);
	wi = vec_neg(wi);

	//etai = hit->mat->refractive_index / ctx->refractive_index;
	//etat = ctx->refractive_index;
	etai = 1.5f;
	etat = 1.0f;
	if (vec_dot(hit->hit.geometric_normal, wiw) < 0)
	{
		etai = 1.0f;
		etat = 1.5f;
		//etai = ctx->refractive_index / hit->mat->refractive_index;
		//etat = hit->mat->refractive_index;
	}
	fresnel = f_dielectric(costheta(result.wo), etai, etat);
	
	result.pdf = 1.0f;
	result.bsdf = vec3(1.0, 1.0, 1.0);
	if (!refract(wi, etai / etat, &result.wo))
	{
		result.wo = vec3(-x(wi), -y(wi), z(wi));
	//	result.bsdf = filter_sample(ctx->world, bxdf->base.tex, hit->hit.uv);
		//fresnel = f_dielectric(costheta(result.wo), etai, etat);
	//	result.bsdf = vec_scale(result.bsdf, fresnel / rt_abs(costheta(result.wo)));
		return (result);
	}

	result.bsdf = vec_mul(filter_sample(ctx->world, bxdf->refraction_tex,
					hit->hit.uv), vec_sub(vec3(1.0f, 1.0f, 1.0f), 
					vec3(fresnel, fresnel, fresnel)));
	result.bsdf = vec_scale(result.bsdf, 1.0f / rt_abs(costheta(result.wo)));
	if (costheta(wi) > 0)
		ctx->refractive_index *= hit->mat->refractive_index;
	else
		ctx->refractive_index /= hit->mat->refractive_index;
	return (result);
}
*/

t_vec
	transmissive_f(t_trace_ctx *ctx, const t_world_hit *hit,
			const GLOBAL t_bxdf_transmissive *bxdf, t_vec wi, t_vec wo)
{
	(void) ctx;
	(void) hit;
	(void) bxdf;
	(void) wi;
	(void) wo;
	return (vec_0());
}

float
	transmissive_pdf(t_trace_ctx *ctx, const t_world_hit *hit,
			const GLOBAL t_bxdf_transmissive *bxdf, t_vec wi, t_vec wo)
{
	(void) ctx;
	(void) hit;
	(void) bxdf;
	(void) wi;
	(void) wo;
	return (0.0f);
}
