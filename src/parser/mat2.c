/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   mat2.c                                         #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:08 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:08 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "mat.h"
#include "world_impl.h"
#include "bsdf.h"

void
	rt_exec_transmissive(t_world *world, t_parse_ctx *ctx)
{
	t_bxdf_transmissive	bxdf;

	rt_check_in_mat(ctx);
	bxdf.base.weight = rt_float(ctx);
	bxdf.base.type = RT_BXDF_TRANSMISSIVE;
	bxdf.base.tex = rt_filter(world, ctx);
	world_insert_bxdf(world, ctx->mat, &bxdf, sizeof(bxdf));
}

void
	rt_exec_emission(t_world *world, t_parse_ctx *ctx)
{
	rt_check_in_mat(ctx);
	ctx->mat->brightness = rt_float(ctx);
	ctx->mat->emission = rt_filter(world, ctx);
	ctx->mat->flags |= RT_MAT_EMITTER;
}

void
	rt_exec_emission_exp(t_world *world, t_parse_ctx *ctx)
{
	(void) world;
	rt_check_in_mat(ctx);
	ctx->mat->emission_exp = rt_float(ctx);
}

void
	rt_exec_smooth(t_world *world, t_parse_ctx *ctx)
{
	(void) world;
	rt_check_in_mat(ctx);
	ctx->mat->flags |= RT_MAT_SMOOTH;
}

void
	rt_exec_alpha(t_world *world, t_parse_ctx *ctx)
{
	rt_check_in_mat(ctx);
	ctx->mat->alpha = rt_filter(world, ctx);
	ctx->mat->flags |= RT_MAT_HAS_ALPHA;
}
