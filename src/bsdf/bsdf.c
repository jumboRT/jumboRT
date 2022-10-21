#include "bsdf.h"

#include "world.h"
#include "mat.h"

static t_vec
	bsdf_f_int(const t_bxdf_ctx *bxdf_ctx, t_vec wo)
{
	uint32_t			idx;
	uint32_t			type;
	const GLOBAL t_bxdf	*bxdf;
	t_vec				result;

	result = vec_0();
	idx = get_bsdf(bxdf_ctx->hit).begin;
	type = 0;
	while (type < RT_BXDF_TYPE_COUNT)
	{
		while (idx < get_bsdf(bxdf_ctx->hit).end)
		{
			bxdf = get_bxdf_const(bxdf_ctx->ctx->world, idx);
			if (bxdf->type != type)
				break ;
			if (bxdf_match(bxdf_ctx, bxdf, wo))
				result = vec_add(bxdf_f(bxdf_ctx, bxdf, wo), result);
			idx++;
		}
		type++;
	}
	result = vec_scale(result,
			get_bsdf(bxdf_ctx->hit).end - get_bsdf(bxdf_ctx->hit).begin);
	return (result);
}

static float
	bsdf_pdf_int(const t_bxdf_ctx *bxdf_ctx,
			t_vec wo)
{
	uint32_t			idx;
	uint32_t			type;
	const GLOBAL t_bxdf	*bxdf;
	float				result[2];

	idx = get_bsdf(bxdf_ctx->hit).begin;
	result[0] = 0.0f;
	result[1] = 0.0f;
	type = 0;
	while (type < RT_BXDF_TYPE_COUNT)
	{
		while (idx < get_bsdf(bxdf_ctx->hit).end)
		{
			bxdf = get_bxdf_const(bxdf_ctx->ctx->world, idx);
			if (bxdf->type != type)
				break ;
			result[0] += bxdf_pdf(bxdf_ctx, bxdf, wo);
			result[1] += 1.0f;
			idx++;
		}
		type++;
	}
	if (result[1] == 0.0f)
		return (0.0f);
	return (result[0] / result[1]);
}

t_vec
	bsdf_f(const t_bxdf_ctx *bxdf_ctx, t_vec wo)
{
	t_bxdf_ctx	new_bxdf_ctx;

	new_bxdf_ctx.ctx = bxdf_ctx->ctx;
	new_bxdf_ctx.hit = bxdf_ctx->hit;
	new_bxdf_ctx.wi = world_to_local(bxdf_ctx->hit, bxdf_ctx->wi);
	wo = world_to_local(bxdf_ctx->hit, wo);
	return (bsdf_f_int(&new_bxdf_ctx, wo));
}

float
	bsdf_pdf(const t_bxdf_ctx *bxdf_ctx, t_vec wo)
{
	t_bxdf_ctx	new_bxdf_ctx;

	new_bxdf_ctx.ctx = bxdf_ctx->ctx;
	new_bxdf_ctx.hit = bxdf_ctx->hit;
	new_bxdf_ctx.wi = world_to_local(bxdf_ctx->hit, bxdf_ctx->wi);
	wo = world_to_local(bxdf_ctx->hit, wo);
	return (bsdf_pdf_int(&new_bxdf_ctx, wo));
}

t_sample
	bsdf_sample(const t_bxdf_ctx *bxdf_ctx)
{
	uint32_t			idx;
	const GLOBAL t_bxdf	*bxdf;
	t_sample			result;
	t_bxdf_ctx			new_bxdf_ctx;

	new_bxdf_ctx.ctx = bxdf_ctx->ctx;
	new_bxdf_ctx.hit = bxdf_ctx->hit;
	if (get_bsdf(bxdf_ctx->hit).end == get_bsdf(bxdf_ctx->hit).begin)
	{
		result.bsdf = vec_0();
		result.wo = vec_0();
		result.pdf = 0.0f;
		result.bxdf = 0;
		return (result);
	}
	idx = rt_random(&bxdf_ctx->ctx->ctx->seed) % (get_bsdf(bxdf_ctx->hit).end
			- get_bsdf(bxdf_ctx->hit).begin);
	bxdf = get_bxdf_const(bxdf_ctx->ctx->world, get_bsdf(bxdf_ctx->hit).begin
			+ idx);
	new_bxdf_ctx.wi = world_to_local(bxdf_ctx->hit, bxdf_ctx->wi);
	result = bxdf_sample_int(&new_bxdf_ctx, bxdf);
	result.wo = local_to_world(bxdf_ctx->hit, result.wo);
	return (result);
}
