/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   mat1.c                                         #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:07 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:07 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "world_impl.h"
#include "bsdf.h"

void
	rt_exec_diffuse(t_world *world, t_parse_ctx *ctx)
{
	t_bxdf_diffuse	bxdf;

	rt_check_in_mat(ctx);
	bxdf.base.type = RT_BXDF_DIFFUSE;
	bxdf.base.weight = rt_float(ctx);
	bxdf.base.tex = rt_filter(world, ctx);
	world_insert_bxdf(world, ctx->mat, &bxdf, sizeof(bxdf));
}

void
	rt_exec_reflective(t_world *world, t_parse_ctx *ctx)
{
	t_bxdf_reflective	bxdf;

	rt_check_in_mat(ctx);
	bxdf.base.type = RT_BXDF_REFLECTIVE;
	bxdf.base.weight = rt_float(ctx);
	bxdf.base.tex = rt_filter(world, ctx);
	world_insert_bxdf(world, ctx->mat, &bxdf, sizeof(bxdf));
}

void
	rt_exec_phong(t_world *world, t_parse_ctx *ctx)
{
	t_bxdf_phong	bxdf;

	rt_check_in_mat(ctx);
	bxdf.base.type = RT_BXDF_PHONG;
	bxdf.base.weight = rt_float(ctx);
	bxdf.base.tex = rt_filter(world, ctx);
	bxdf.roughness = rt_filter(world, ctx);
	world_insert_bxdf(world, ctx->mat, &bxdf, sizeof(bxdf));
}

void
	rt_exec_oren_nayar(t_world *world, t_parse_ctx *ctx)
{
	t_bxdf_oren_nayar	bxdf;
	float				sigma;

	rt_check_in_mat(ctx);
	bxdf.base.type = RT_BXDF_OREN_NAYAR;
	bxdf.base.weight = rt_float(ctx);
	bxdf.base.tex = rt_filter(world, ctx);
	sigma = rt_degtorad(rt_float(ctx));
	sigma = sigma * sigma;
	bxdf.alpha = 1.0f - (sigma / (2.0f * (sigma + 0.33f)));
	bxdf.beta = 0.45f * sigma / (sigma + 0.09f);
	world_insert_bxdf(world, ctx->mat, &bxdf, sizeof(bxdf));
}

void
	rt_exec_specular(t_world *world, t_parse_ctx *ctx)
{
	t_bxdf_specular	bxdf;

	rt_check_in_mat(ctx);
	bxdf.base.weight = rt_float(ctx);
	bxdf.base.type = RT_BXDF_SPECULAR;
	bxdf.base.tex = rt_filter(world, ctx);
	world_insert_bxdf(world, ctx->mat, &bxdf, sizeof(bxdf));
}
