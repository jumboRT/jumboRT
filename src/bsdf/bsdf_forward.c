/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   bsdf_forward.c                                 #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:18 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:18 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "bsdf.h"

t_vec
	bxdf_f(const t_bxdf_ctx *bxdf_ctx, const GLOBAL t_bxdf *bxdf, t_vec wo)
{
	t_vec	result;

	result = vec_0();
	if (bxdf->type == RT_BXDF_DIFFUSE)
		result = diffuse_f(bxdf_ctx,
				(const GLOBAL t_bxdf_diffuse*) bxdf, wo);
	else if (bxdf->type == RT_BXDF_REFLECTIVE)
		result = reflective_f(bxdf_ctx,
				(const GLOBAL t_bxdf_reflective*) bxdf, wo);
	else if (bxdf->type == RT_BXDF_TRANSMISSIVE)
		result = transmissive_f(bxdf_ctx,
				(const GLOBAL t_bxdf_transmissive*) bxdf, wo);
	else if (bxdf->type == RT_BXDF_SPECULAR)
		result = specular_f(bxdf_ctx,
				(const GLOBAL t_bxdf_specular*) bxdf, wo);
	else if (bxdf->type == RT_BXDF_PHONG)
		result = phong_f(bxdf_ctx,
				(const GLOBAL t_bxdf_phong*) bxdf, wo);
	else if (bxdf->type == RT_BXDF_OREN_NAYAR)
		result = oren_nayar_f(bxdf_ctx,
				(const GLOBAL t_bxdf_oren_nayar*) bxdf, wo);
	return (vec_scale(result, bxdf->weight));
}

float
	bxdf_pdf(const t_bxdf_ctx *bxdf_ctx, const GLOBAL t_bxdf *bxdf, t_vec wo)
{
	float	result;

	result = 0.0f;
	if (bxdf->type == RT_BXDF_DIFFUSE)
		result += diffuse_pdf(bxdf_ctx,
				(const GLOBAL t_bxdf_diffuse *) bxdf, wo);
	else if (bxdf->type == RT_BXDF_REFLECTIVE)
		result += reflective_pdf(bxdf_ctx,
				(const GLOBAL t_bxdf_reflective *) bxdf, wo);
	else if (bxdf->type == RT_BXDF_TRANSMISSIVE)
		result += transmissive_pdf(bxdf_ctx,
				(const GLOBAL t_bxdf_transmissive *) bxdf, wo);
	else if (bxdf->type == RT_BXDF_SPECULAR)
		result += specular_pdf(bxdf_ctx,
				(const GLOBAL t_bxdf_specular *) bxdf, wo);
	else if (bxdf->type == RT_BXDF_PHONG)
		result += phong_pdf(bxdf_ctx,
				(const GLOBAL t_bxdf_phong *) bxdf, wo);
	else if (bxdf->type == RT_BXDF_OREN_NAYAR)
		result += oren_nayar_pdf(bxdf_ctx,
				(const GLOBAL t_bxdf_oren_nayar *) bxdf, wo);
	return (result);
}

t_sample
	bxdf_sample(const t_bxdf_ctx *bxdf_ctx,
			const GLOBAL t_bxdf *bxdf)
{
	t_sample	result;

	if (bxdf->type == RT_BXDF_DIFFUSE)
		result = diffuse_sample(bxdf_ctx,
				(const GLOBAL t_bxdf_diffuse*) bxdf);
	else if (bxdf->type == RT_BXDF_REFLECTIVE)
		result = reflective_sample(bxdf_ctx,
				(const GLOBAL t_bxdf_reflective*) bxdf);
	else if (bxdf->type == RT_BXDF_TRANSMISSIVE)
		result = transmissive_sample(bxdf_ctx,
				(const GLOBAL t_bxdf_transmissive*) bxdf);
	else if (bxdf->type == RT_BXDF_SPECULAR)
		result = specular_sample(bxdf_ctx,
				(const GLOBAL t_bxdf_specular*) bxdf);
	else if (bxdf->type == RT_BXDF_PHONG)
		result = phong_sample(bxdf_ctx,
				(const GLOBAL t_bxdf_phong*) bxdf);
	else if (bxdf->type == RT_BXDF_OREN_NAYAR)
		result = oren_nayar_sample(bxdf_ctx,
				(const GLOBAL t_bxdf_oren_nayar*) bxdf);
	result.bsdf = vec_scale(result.bsdf, bxdf->weight);
	result.bxdf = bxdf;
	return (result);
}
