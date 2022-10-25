/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   bsdf_sample_int.c                              #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:18 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:18 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "bsdf.h"
#include "world.h"

static t_uint32
	bxdf_sample_int_start(const t_bxdf_ctx *bxdf_ctx,
			const GLOBAL t_bxdf *bxdf)
{
	t_uint32			start;
	const GLOBAL t_bxdf	*bxdf2;

	start = get_bsdf(bxdf_ctx->hit).begin;
	while (start < get_bsdf(bxdf_ctx->hit).end)
	{
		bxdf2 = get_bxdf_const(bxdf_ctx->ctx->world, start);
		if (bxdf2->type >= bxdf->type)
			break ;
		start += 1;
	}
	return (start);
}

static void
	bxdf_sample_int_pdf(const t_bxdf_ctx *bxdf_ctx,
			const GLOBAL t_bxdf *bxdf, t_uint32 start, t_sample *result)
{
	t_uint32			idx;
	const GLOBAL t_bxdf	*bxdf2;

	idx = start;
	while (idx < get_bsdf(bxdf_ctx->hit).end)
	{
		bxdf2 = get_bxdf_const(bxdf_ctx->ctx->world, idx);
		if (bxdf2->type > bxdf->type)
			break ;
		if (bxdf != bxdf2)
			result->pdf += bxdf_pdf(bxdf_ctx, bxdf2, result->wo);
		idx += 1;
	}
	result->pdf /= idx - start;
}

static void
	bxdf_sample_int_bsdf(const t_bxdf_ctx *bxdf_ctx,
			const GLOBAL t_bxdf *bxdf, t_uint32 start, t_sample *result)
{
	t_uint32			idx;
	const GLOBAL t_bxdf	*bxdf2;

	result->bsdf = vec_0();
	idx = start;
	while (idx < get_bsdf(bxdf_ctx->hit).end)
	{
		bxdf2 = get_bxdf_const(bxdf_ctx->ctx->world, idx);
		if (bxdf2->type > bxdf->type)
			break ;
		if (bxdf_match(bxdf_ctx, bxdf2, result->wo))
			result->bsdf = vec_add(result->bsdf,
					bxdf_f(bxdf_ctx, bxdf2, result->wo));
		idx += 1;
	}
}

t_sample
	bxdf_sample_int(const t_bxdf_ctx *bxdf_ctx,
			const GLOBAL t_bxdf *bxdf)
{
	t_uint32			start;
	t_sample			result;

	result = bxdf_sample(bxdf_ctx, bxdf);
	result.bsdf = vec_scale(result.bsdf, bxdf->weight);
	if (!bxdf_is_perfspec(bxdf))
	{
		start = bxdf_sample_int_start(bxdf_ctx, bxdf);
		bxdf_sample_int_pdf(bxdf_ctx, bxdf, start, &result);
		bxdf_sample_int_bsdf(bxdf_ctx, bxdf, start, &result);
	}
	result.bsdf = vec_scale(result.bsdf, get_bsdf(bxdf_ctx->hit).end
			- get_bsdf(bxdf_ctx->hit).begin);
	return (result);
}
