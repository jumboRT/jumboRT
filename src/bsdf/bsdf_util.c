/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   bsdf_util.c                                    #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:18 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:18 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "bsdf.h"

#include "mat.h"
#include "world.h"

t_bsdf
	get_bsdf(const t_world_hit *hit)
{
	if (hit->is_volume)
		return (hit->mat->volume);
	else
		return (hit->mat->surface);
}

t_int32
	bxdf_is_perfspec(const GLOBAL t_bxdf *bxdf)
{
	return (bxdf->type == RT_BXDF_REFLECTIVE
		|| bxdf->type == RT_BXDF_TRANSMISSIVE
		|| bxdf->type == RT_BXDF_SPECULAR);
}

static t_int32
	bxdf_is_reflective(const GLOBAL t_bxdf *bxdf)
{
	return (bxdf->type == RT_BXDF_DIFFUSE
		|| bxdf->type == RT_BXDF_REFLECTIVE
		|| bxdf->type == RT_BXDF_MF_REFLECTIVE
		|| bxdf->type == RT_BXDF_COOK_TORRANCE
		|| bxdf->type == RT_BXDF_BLINN_PHONG
		|| bxdf->type == RT_BXDF_PHONG
		|| bxdf->type == RT_BXDF_SPECULAR
		|| bxdf->type == RT_BXDF_OREN_NAYAR);
}

static t_int32
	bxdf_is_transmissive(const GLOBAL t_bxdf *bxdf)
{
	return (bxdf->type == RT_BXDF_TRANSMISSIVE);
}

t_int32
	bxdf_match(const t_bxdf_ctx *bxdf_ctx,
			const GLOBAL t_bxdf *bxdf, t_vec wo)
{
	int		reflect;

	reflect = vec_dot(bxdf_ctx->gn, bxdf_ctx->wi)
		* vec_dot(bxdf_ctx->gn, wo) < 0;
	return ((reflect && bxdf_is_reflective(bxdf))
		|| (!reflect && bxdf_is_transmissive(bxdf)));
}
