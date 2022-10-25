/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   specular.c                                     #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:17 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:17 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "bsdf.h"
#include "world.h"
#include "mat.h"

/* ODOT: this is an ugly hack but we don't care
if (fresnel > 1.0f - RT_TINY_VAL)
	return (result);
*/
t_sample
	specular_sample(const t_bxdf_ctx *ctx,
			const GLOBAL t_bxdf_specular *bxdf)
{
	t_sample	result;
	t_vec		color;
	float		fresnel;

	result.wo = vec3(x(ctx->wi), y(ctx->wi), -z(ctx->wi));
	result.bsdf = vec_0();
	result.pdf = 0.0f;
	fresnel = f_dielectric(costheta(result.wo), 1.0f,
			ctx->hit->mat->refractive_index);
	if (fresnel > 1.0f - RT_TINY_VAL)
		return (result);
	color = vec_scale(filter_sample(ctx->ctx->world, bxdf->base.tex,
				ctx->hit->hit.uv), fresnel);
	if (vec_dot(ctx->gn, ctx->wi) * vec_dot(ctx->gn, result.wo) < 0)
	{
		result.bsdf = vec_scale(color, 1.0f / rt_abs(costheta(result.wo)));
		result.pdf = 1.0f;
	}
	return (result);
}

t_vec
	specular_f(const t_bxdf_ctx *ctx,
			const GLOBAL t_bxdf_specular *bxdf, t_vec wo)
{
	(void) ctx;
	(void) bxdf;
	(void) wo;
	return (vec_0());
}

float
	specular_pdf(const t_bxdf_ctx *ctx,
			const GLOBAL t_bxdf_specular *bxdf, t_vec wo)
{
	(void) ctx;
	(void) bxdf;
	(void) wo;
	return (0.0f);
}
