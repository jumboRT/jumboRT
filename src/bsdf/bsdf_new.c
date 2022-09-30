#include "bsdf.h"

#include "world.h"
#include "mat.h"

static t_sample
	bxdf_sample_single(t_trace_ctx *ctx, const t_world_hit *hit,
			const GLOBAL t_bxdf *bxdf, t_vec wiw)
{
	t_sample	result;

	if (bxdf->type == RT_BXDF_DIFFUSE)
		result = diffuse_sample(ctx, hit, (const GLOBAL t_bxdf_diffuse*) bxdf,
					wiw);
	result.wo = local_to_world(hit, result.wo);
	result.bxdf = bxdf;
	return (result);
}

static t_sample
	bxdf_sample(t_trace_ctx *ctx, const t_world_hit *hit, uint32_t bxdf_idx, 
			t_vec wiw)
{
	/*
	uint32_t	idx;
	uint32_t	type;
	float		pdf;
	*/
	t_sample	result;

	result = bxdf_sample_single(ctx, hit, get_bxdf_const(ctx->world, bxdf_idx),
			wiw);	
	/*
	idx = hit->mat->surface.begin;
	type = 0;
	pdf = 0.0f;
	while (type < RT_BXDF_COUNT) 
	{
		while (idx < hit->mat->surface.end)
		{
			if (idx != bxdf_idx)
				pdf += bxdf_pdf(ctx, hit, get_bxdf_const(ctx->world, idx));
			idx += 1;
		}
		type += 1;
	}
	result.pdf += pdf;
	*/
	return (result);
}

t_sample
	sample(t_vec bsdf, t_vec wo, float pdf)
{
	t_sample	result;

	result.bsdf = bsdf;
	result.wo = wo;
	result.pdf = pdf;
	return (result);
}

float
	bxdf_pdf(t_trace_ctx *ctx, const t_world_hit *hit,
			const GLOBAL t_bxdf *bxdf, t_vec wi, t_vec wo)
{
	float	result;

	result = 0.0f;
	if (bxdf->type == RT_BXDF_DIFFUSE)
		result += diffuse_pdf(ctx, hit, (const GLOBAL t_bxdf_diffuse *) bxdf,
						wi, wo);
	return (result);
}

float
	bsdf_pdf(t_trace_ctx *ctx, const t_world_hit *hit, t_vec wi, t_vec wo)
{

	uint32_t			idx;
	uint32_t			type;
	const GLOBAL t_bxdf	*bxdf;
	float				result;

	wi = world_to_local(hit, wi);
	wo = world_to_local(hit, wo);
	result = 0.0f;
	idx = hit->mat->surface.begin;
	type = 0;
	while (type < RT_BXDF_COUNT) /* TODO rename to BXDF_TYPE_COUNT */
	{
		while (idx < hit->mat->surface.end)
		{
			bxdf = get_bxdf_const(ctx->world, idx);
			if (bxdf->type != type)
				break;
			result += bxdf_pdf(ctx, hit, bxdf, wi, wo) * bxdf->weight;
			idx++;
		}
		type++;
	}
	return (result);
}

t_vec
	bxdf_f(t_trace_ctx *ctx, const t_world_hit *hit, const GLOBAL t_bxdf *bxdf,
			t_vec wi, t_vec wo)
{
	t_vec	result;

	result = vec_0();
	if (bxdf->type == RT_BXDF_DIFFUSE)
		result =
			diffuse_f(ctx, hit, (const GLOBAL t_bxdf_diffuse*) bxdf, wi, wo);
	return (result);
}

t_vec
	bsdf_f(t_trace_ctx *ctx, const t_world_hit *hit, t_vec wi, t_vec wo)
{
	uint32_t			idx;
	uint32_t			type;
	const GLOBAL t_bxdf	*bxdf;
	t_vec				result;

	result = vec_0();
	wi = world_to_local(hit, wi);
	wo = world_to_local(hit, wo);
	idx = hit->mat->surface.begin;
	type = 0;
	while (type < RT_BXDF_COUNT) /* TODO rename to BXDF_TYPE_COUNT */
	{
		while (idx < hit->mat->surface.end)
		{
			bxdf = get_bxdf_const(ctx->world, idx);
			if (bxdf->type != type)
				break;
			result = vec_add(vec_scale(bxdf_f(ctx, hit, bxdf, wi, wo),
						bxdf->weight), result);
			idx++;
		}
		type++;
	}
	return (result);
}

t_sample
	bsdf_sample(t_trace_ctx *ctx, const t_world_hit *hit, t_vec wi)
{
	uint32_t			idx;
	uint32_t			type;
	float				epsilon;
	const GLOBAL t_bxdf	*bxdf;

	epsilon = rt_random_float(&ctx->ctx->seed);
	idx = hit->mat->surface.begin;
	type = 0;
	wi = world_to_local(hit, wi);
	while (type < RT_BXDF_COUNT) /* TODO rename to BXDF_TYPE_COUNT */
	{
		while (idx < hit->mat->surface.end)
		{
			bxdf = get_bxdf_const(ctx->world, idx);
			if (bxdf->type != type)
				break;
			epsilon -= bxdf->weight;
			if (epsilon <= 0)
				return (bxdf_sample(ctx, hit, idx, wi));
			idx += 1;
		}
		type += 1;
	}
	return (sample(vec_0(), vec_0(), 1.0f));
}

int32_t
	bxdf_is_perfspec(const GLOBAL t_bxdf *bxdf)
{
	return (0); /* TODO implement */
}
