#include "bsdf.h"

#include "world.h"
#include "mat.h"

static int
	refract(t_vec wi, t_vec n, float eta, t_vec *wt)
{
	float	costheta = rt_min(vec_dot(wi, n), 1.0f);
	t_vec	rperp = vec_scale(vec_add(wi, vec_scale(n, costheta)), eta);
	t_vec	rparl = vec_scale(n, -rt_sqrt(rt_abs(1.0f - vec_mag2(rperp))));
	*wt = vec_add(rperp, rparl);
	return (1);
	/*
	float	costhetai;
	float	costhetat;
	float	sin2thetai;
	float	sin2thetat;

	costhetai = vec_dot(n, wi);
	sin2thetai = rt_max(0.0f, 1.0f - costhetai * costhetai);
	sin2thetat = eta * eta * sin2thetai;
	if (sin2thetat >= 1)
		return (0);
	costhetat = rt_sqrt(1.0f - sin2thetat);
	*wt = vec_add(vec_scale(vec_neg(wi), eta),
			vec_scale(n, eta * costhetai * costhetat));
	return (1);
	*/
}

static t_vec
	reflect(t_vec wi, t_vec n)
{
	return (vec_add(vec_neg(wi), vec_scale(n, vec_dot(wi, n) * 2.0f)));
}

static t_vec
	forward(t_vec n, t_vec v)
{
	if (vec_dot(n, v) < 0.0f)
		return (vec_neg(n));
	return (n);
}

float
	f_dielectric(float costhetai, float etai, float etat)
{
	float	sinthetai;
	float	sinthetat;
	float	costhetat;
	float	rparl;
	float	rperp;

	costhetai = rt_clamp(costhetai, -1.0f, 1.0f);

	sinthetai = rt_sqrt(rt_max(0.0f, 1.0f - costhetai * costhetai));
	sinthetat = etai / etat * sinthetai;

	if (sinthetat >= 1.0f)
		return (1);
	costhetat = rt_sqrt(rt_max(0.0f, 1.0f - sinthetat * sinthetat));
	rparl = ((etat * costhetai) - (etai * costhetat)) /
		((etat * costhetai) + (etai * costhetat));
	rperp = ((etai * costhetai) - (etat * costhetat)) /
		((etai * costhetai) + (etat * costhetat));
	return ((rparl * rparl + rperp * rperp) / 2.0f);
}

t_sample
	transmissive_sample(t_trace_ctx *ctx, const t_world_hit *hit,
			const GLOBAL t_bxdf_transmissive *bxdf, t_vec wi)
{
	t_sample	result;
	float		etai;
	float		etat;
	//float		fresnel;

	wi = vec_neg(wi);

	//etai = hit->mat->refractive_index / ctx->refractive_index;
	//etat = ctx->refractive_index;
	etai = 1.0f;
	etat = 1.5f;
	if (costheta(wi) > 0)
	{
		etai = 1.5f;
		etat = 1.0f;
		//etai = ctx->refractive_index / hit->mat->refractive_index;
		//etat = hit->mat->refractive_index;
	}
	refract(wi, forward(vec_z(1.0f), wi), etai / etat, &result.wo);

	result.pdf = 1;
	result.bsdf = vec3(1.0f, 1.0f, 1.0f);
	return (result);
	//fresnel = f_dielectric(costheta(result.wo), etai, etat);
	
	/*
	if (!refract(wi, forward(vec_z(1.0f), wi), etai / etat, &result.wo))
	{
		result.wo = vec3(-x(wi), -y(wi), z(wi));
		result.wo = reflect(wi, forward(vec_z(1.0f), wi)); //TODO remove if not working
		result.pdf = 1.0f;
		result.bsdf = filter_sample(ctx->world, bxdf->base.tex, hit->hit.uv);
		fresnel = f_dielectric(costheta(result.wo), etai, etat);
		result.bsdf = vec_scale(result.bsdf, fresnel / rt_abs(costheta(result.wo)));
		return (result);
	}
	*/

	/*
	result.bsdf = vec_mul(filter_sample(ctx->world, bxdf->refraction_tex,
					hit->hit.uv), vec_sub(vec3(1.0f, 1.0f, 1.0f), 
					vec3(fresnel, fresnel, fresnel)));
	result.bsdf = vec_scale(result.bsdf, 1.0f / rt_abs(costheta(result.wo)));
	*/
	/*
	if (costheta(wi) > 0)
		ctx->refractive_index *= hit->mat->refractive_index;
	else
		ctx->refractive_index /= hit->mat->refractive_index;
	*/
	return (result);
}

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
