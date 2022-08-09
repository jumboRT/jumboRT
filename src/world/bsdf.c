#include "world.h"

static t_vec
	reflect(t_vec wiw, t_vec n)
{
	return (vec_sub(wiw, vec_scale(n, 2.0 * vec_dot(wiw, n))));
}

static FLOAT
	reflectance(FLOAT costheta, FLOAT etai_etat)
{
	FLOAT	r0;
	
	r0 = (1.0 - etai_etat) / (1.0 + etai_etat);
	r0 = r0 * r0;
	return (r0 + (1.0 - r0) * rt_pow(1.0 - costheta, 5.0));
}

static t_vec
	f_bxdf_diffuse(const GLOBAL t_world *world, const GLOBAL t_bxdf_diffuse *bxdf, t_world_hit hit, t_vec wi, t_vec wo)
{
	t_vec	color;

	(void) wi;
	(void) wo;
	color = filter_sample(world, bxdf->base.tex, hit.hit.uv);
	return (vec_scale(color, RT_1_PI * 2));
}

static t_vec
	f_bxdf_reflective_sample(const GLOBAL t_world *world, GLOBAL t_context *ctx, const GLOBAL t_bxdf_reflective *bxdf, t_world_hit hit, t_vec wi, t_vec *wo)
{
	(void)ctx;
	(void) bxdf;
	*wo = reflect(wi, hit.relative_normal);
	*wo = vec_add(*wo, vec_scale(rt_random_in_sphere(&ctx->seed), bxdf->fuzzy));
	return (filter_sample(world, bxdf->base.tex, hit.hit.uv));
}

static t_vec
	f_bxdf_diffuse_sample(const GLOBAL t_world *world, GLOBAL t_context *ctx, const GLOBAL t_bxdf_diffuse *bxdf, t_world_hit hit, t_vec wi, t_vec *wo)
{
	while (1)
	{
		*wo = rt_random_on_hemi(&ctx->seed, hit.relative_normal);
		if (vec_dot(*wo, hit.geometric_normal) >= 0)
			break;
	}
	return (f_bxdf_diffuse(world, bxdf, hit, wi, *wo));
}

static t_vec
	refract(t_vec wi, t_vec n, FLOAT etai_etat)
{
	FLOAT	costheta;
	FLOAT	tmp;
	t_vec	rperp;
	t_vec	rparl;

	costheta = rt_min(vec_dot(vec_neg(wi), n), 1.0);
	rperp = vec_scale(vec_add(vec_scale(n, costheta), wi), etai_etat);
	tmp = -rt_sqrt(rt_abs(1.0 - vec_mag2(rperp)));
	rparl = vec_scale(n, tmp);
	return (vec_add(rperp, rparl));
}

static t_vec f_bxdf_transmissive_sample(const GLOBAL t_world *world, GLOBAL t_context *ctx, t_trace_ctx *trace_ctx,
					  const GLOBAL t_bxdf_transmissive *bxdf, t_world_hit hit, t_vec wiw, t_vec *wow)
{
	t_vec	color;
	FLOAT	etai_etat;
	FLOAT	costheta;
	FLOAT	sintheta;

	etai_etat = bxdf->eta;
	if (vec_dot(hit.relative_normal, hit.hit.normal) > 0.0)
		etai_etat = 1.0 / bxdf->eta;
	costheta = rt_min(vec_dot(vec_neg(wiw), hit.relative_normal), 1.0);
	sintheta = rt_sqrt(1.0 - costheta*costheta);
	color = filter_sample(world, bxdf->base.tex, hit.hit.uv);
	if (reflectance(costheta, etai_etat) > rt_random_float(&ctx->seed) || etai_etat * sintheta > 1.0)
	{
		*wow = reflect(wiw, hit.relative_normal);
		/**wow = vec_sub(wiw, vec_scale(hit.relative_normal, 2.0 * vec_dot(wiw, hit.relative_normal)));*/
	}
	else
	{
		*wow = vec_norm(refract(wiw, hit.relative_normal, etai_etat));
	}
	return (color);
}

static t_vec
	f_bxdf_sample(const GLOBAL t_world *world, GLOBAL t_context *ctx, t_trace_ctx *trace_ctx, const GLOBAL t_bxdf *bxdf, t_world_hit hit, t_vec wiw, t_vec *wow)
{
	t_vec	result;

	result = vec_0();
	if (bxdf->type == RT_BXDF_DIFFUSE)
		result = f_bxdf_diffuse_sample(world, ctx, (const GLOBAL t_bxdf_diffuse *) bxdf, hit, wiw, wow);
	if (bxdf->type == RT_BXDF_REFLECTIVE)
		result = f_bxdf_reflective_sample(world, ctx, (const GLOBAL t_bxdf_reflective *) bxdf, hit, wiw, wow);
	if (bxdf->type == RT_BXDF_TRANSMISSIVE)
		return (f_bxdf_transmissive_sample(world, ctx, trace_ctx, (const GLOBAL t_bxdf_transmissive *) bxdf, hit, wiw, wow));;
	return result;
}

t_vec
	f_bsdf_sample(const GLOBAL t_world *world, GLOBAL t_context *ctx, t_trace_ctx *trace_ctx, t_bsdf bsdf, t_world_hit hit, t_vec wiw, t_vec *wow)
{
	uint32_t			idx;
	FLOAT				rand;
	const GLOBAL t_bxdf	*bxdf;
	uint32_t			type;

	idx = bsdf.begin;
	rand = rt_random_float_range(&ctx->seed, 0.0, bsdf.weight);
	type = 0;
	while (type < RT_BXDF_COUNT)
	{
		while (idx < bsdf.end)
		{
			bxdf = get_bxdf_const(world, idx);
			if (bxdf->type != type)
				break ;
			rand -= bxdf->weight;
			if (rand <= 0)
				return (vec_scale(f_bxdf_sample(world, ctx, trace_ctx, bxdf, hit, wiw, wow), bsdf.weight));
			idx += 1;
		}
		type += 1;
	}
	return (vec(0, 0, 0, 0));
}
