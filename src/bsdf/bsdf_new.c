#include "bsdf.h"

#include "world.h"
#include "mat.h"

static t_bsdf
	get_bsdf(const t_world_hit *hit)
{
	if (hit->is_volume)
		return (hit->mat->volume);
	else
		return (hit->mat->surface);
}

t_vec
	bxdf_f(t_trace_ctx *ctx, const t_world_hit *hit, const GLOBAL t_bxdf *bxdf,
			t_vec wi, t_vec wo)
{
	t_vec	result;

	result = vec_0();
	if (bxdf->type == RT_BXDF_DIFFUSE)
		result = diffuse_f(ctx, hit,
				(const GLOBAL t_bxdf_diffuse*) bxdf, wi, wo);
	else if (bxdf->type == RT_BXDF_REFLECTIVE)
		result = reflective_f(ctx, hit,
				(const GLOBAL t_bxdf_reflective*) bxdf, wi, wo);
	else if (bxdf->type == RT_BXDF_TRANSMISSIVE)
		result = transmissive_f(ctx, hit,
				(const GLOBAL t_bxdf_transmissive*) bxdf, wi, wo);
	return (vec_scale(result, bxdf->weight));
}

float
	bxdf_pdf(t_trace_ctx *ctx, const t_world_hit *hit,
			const GLOBAL t_bxdf *bxdf, t_vec wi, t_vec wo)
{
	float	result;

	result = 0.0f;
	if (bxdf->type == RT_BXDF_DIFFUSE)
		result += diffuse_pdf(ctx, hit,
				(const GLOBAL t_bxdf_diffuse *) bxdf, wi, wo);
	else if (bxdf->type == RT_BXDF_REFLECTIVE)
		result += reflective_pdf(ctx, hit,
				(const GLOBAL t_bxdf_reflective *) bxdf, wi, wo);
	else if (bxdf->type == RT_BXDF_TRANSMISSIVE)
		result += transmissive_pdf(ctx, hit,
				(const GLOBAL t_bxdf_transmissive *) bxdf, wi, wo);
	return (result);
}

t_sample
	bxdf_sample(t_trace_ctx *ctx, const t_world_hit *hit,
			const GLOBAL t_bxdf *bxdf, t_vec wiw)
{
	t_sample	result;

	if (bxdf->type == RT_BXDF_DIFFUSE)
		result = diffuse_sample(ctx, hit,
				(const GLOBAL t_bxdf_diffuse*) bxdf, wiw);
	else if (bxdf->type == RT_BXDF_REFLECTIVE)
		result = reflective_sample(ctx, hit,
				(const GLOBAL t_bxdf_reflective*) bxdf, wiw);
	else if (bxdf->type == RT_BXDF_TRANSMISSIVE)
		result = transmissive_sample(ctx, hit,
				(const GLOBAL t_bxdf_transmissive*) bxdf, wiw);
	result.wo = local_to_world(hit, result.wo);
	result.bsdf = vec_scale(result.bsdf, bxdf->weight);
	result.bxdf = bxdf;
	return (result);
}

static t_vec
	bsdf_f_int(t_trace_ctx *ctx, const t_world_hit *hit,
			t_vec wi, t_vec wo)
{
	uint32_t			idx;
	uint32_t			type;
	const GLOBAL t_bxdf	*bxdf;
	t_vec				result;

	result = vec_0();
	idx = get_bsdf(hit).begin;
	type = 0;
	/* TODO: check if this is still faster with an increasing number of bxdfs */
	while (type < RT_BXDF_COUNT) /* TODO rename to BXDF_TYPE_COUNT */
	{
		while (idx < get_bsdf(hit).end)
		{
			bxdf = get_bxdf_const(ctx->world, idx);
			if (bxdf->type != type)
				break ;
			result = vec_add(bxdf_f(ctx, hit, bxdf, wi, wo), result);
			idx++;
		}
		type++;
	}
	return (result);
}

static float
	bsdf_pdf_int(t_trace_ctx *ctx, const t_world_hit *hit,
			t_vec wi, t_vec wo)
{
	uint32_t			idx;
	uint32_t			type;
	uint32_t			match;
	const GLOBAL t_bxdf	*bxdf;
	float				result;

	result = 0.0f;
	idx = get_bsdf(hit).begin;
	type = 0;
	match = 0;
	/* TODO: check if this is still faster with an increasing number of bxdfs */
	while (type < RT_BXDF_COUNT) /* TODO rename to BXDF_TYPE_COUNT */
	{
		while (idx < get_bsdf(hit).end)
		{
			bxdf = get_bxdf_const(ctx->world, idx);
			if (bxdf->type != type)
				break;
			match += 1;
			result += bxdf_pdf(ctx, hit, bxdf, wi, wo);
			idx++;
		}
		type++;
	}
	if (match == 0)
		return (0);
	return (result / match);
}

static t_sample
	bxdf_sample_int(t_trace_ctx *ctx, const t_world_hit *hit,
			const GLOBAL t_bxdf *bxdf, t_vec wiw)
{
	uint32_t			start;
	uint32_t			idx;
	const GLOBAL t_bxdf	*bxdf2;
	t_sample			result;

	result = bxdf_sample(ctx, hit, bxdf, wiw);
	if (!bxdf_is_perfspec(bxdf))
	{
		/* TODO: optimize these loops */
		start = get_bsdf(hit).begin;
		while (start < get_bsdf(hit).end)
		{
			bxdf2 = get_bxdf_const(ctx->world, start);
			if (bxdf2->type >= bxdf->type)
				break ;
			start += 1;
		}
		idx = start;
		while (idx < get_bsdf(hit).end)
		{
			bxdf2 = get_bxdf_const(ctx->world, idx);
			if (bxdf2->type > bxdf->type)
				break ;
			if (bxdf != bxdf2)
				result.pdf += bxdf_pdf(ctx, hit, bxdf2, wiw, result.wo);
			idx += 1;
		}
		result.pdf /= idx - start;
		result.bsdf = vec_0();
		idx = start;
		while (idx < get_bsdf(hit).end)
		{
			bxdf2 = get_bxdf_const(ctx->world, idx);
			if (bxdf2->type > bxdf->type)
				break ;
			result.bsdf = vec_add(result.bsdf, bxdf_f(ctx, hit, bxdf2, wiw, result.wo));
			idx += 1;
		}
	}
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

t_vec
	bsdf_f(t_trace_ctx *ctx, const t_world_hit *hit, t_vec wi, t_vec wo)
{
	wi = world_to_local(hit, wi);
	wo = world_to_local(hit, wo);
	return (bsdf_f_int(ctx, hit, wi, wo));
}

float
	bsdf_pdf(t_trace_ctx *ctx, const t_world_hit *hit, t_vec wi, t_vec wo)
{
	wi = world_to_local(hit, wi);
	wo = world_to_local(hit, wo);
	return (bsdf_pdf_int(ctx, hit, wi, wo));
}

t_sample
	bsdf_sample(t_trace_ctx *ctx, const t_world_hit *hit, t_vec wi)
{
	uint32_t			idx;
	const GLOBAL t_bxdf	*bxdf;

	if (get_bsdf(hit).end == get_bsdf(hit).begin)
		return (sample(vec_0(), vec_0(), 0.0f));
	idx = rt_random(&ctx->ctx->seed) % (get_bsdf(hit).end - get_bsdf(hit).begin);
	bxdf = get_bxdf_const(ctx->world, idx);
	wi = world_to_local(hit, wi);
	return (bxdf_sample_int(ctx, hit, bxdf, wi));
}

int32_t
	bxdf_is_perfspec(const GLOBAL t_bxdf *bxdf)
{
	return (bxdf->type == RT_BXDF_REFLECTIVE || bxdf->type == RT_BXDF_TRANSMISSIVE);
}
