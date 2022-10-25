/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   reflective.c                                   #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:18 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:18 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "bsdf.h"
#include "world.h"

t_sample
	reflective_sample(const t_bxdf_ctx *ctx,
			const GLOBAL t_bxdf_reflective *bxdf)
{
	t_sample	result;
	t_vec		color;

	result.wo = vec_set(ctx->wi, 2, -z(ctx->wi));
	color = filter_sample(ctx->ctx->world, bxdf->base.tex, ctx->hit->hit.uv);
	result.bsdf = vec_0();
	result.pdf = 0.0f;
	if (vec_dot(ctx->gn, ctx->wi) * vec_dot(ctx->gn, result.wo) < 0)
	{
		result.bsdf = vec_scale(color, 1.0f / rt_abs(z(result.wo)));
		result.pdf = 1.0f;
	}
	return (result);
}

t_vec
	reflective_f(const t_bxdf_ctx *ctx,
			const GLOBAL t_bxdf_reflective *bxdf, t_vec wow)
{
	(void) ctx;
	(void) bxdf;
	(void) wow;
	return (vec_0());
}

float
	reflective_pdf(const t_bxdf_ctx *ctx,
			const GLOBAL t_bxdf_reflective *bxdf, t_vec wow)
{
	(void) ctx;
	(void) bxdf;
	(void) wow;
	return (0);
}
