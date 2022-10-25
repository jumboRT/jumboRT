/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   transmissive.c                                 #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:17 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 13:33:38 by csteenvo            :      ..            */
/*                                                                            */
/* ************************************************************************** */

#include "bsdf.h"

#include "world.h"
#include "mat.h"

static int
	refract(t_vec wi, t_vec n, float eta, t_vec *wt)
{
	float	costhetai;
	float	costhetat;
	float	sin2thetai;
	float	sin2thetat;

	costhetai = vec_dot(wi, n);
	sin2thetai = rt_max(0.0f, 1.0f - costhetai * costhetai);
	sin2thetat = eta * eta * sin2thetai;
	if (sin2thetat >= 1)
		return (0);
	costhetat = rt_sqrt(1.0f - sin2thetat);
	*wt = vec_add(vec_scale(vec_neg(wi), eta),
			vec_scale(n, eta * costhetai - costhetat));
	return (1);
}

static t_vec
	face_forward(t_vec n, t_vec v)
{
	if (vec_dot(n, v) < 0)
		return (vec_neg(n));
	return (n);
}

t_sample
	transmissive_sample(const t_bxdf_ctx *ctx,
			const GLOBAL t_bxdf_transmissive *bxdf)
{
	float		eta;
	float		fresnel;
	t_sample	result;
	t_vec		wi;

	wi = vec_neg(ctx->wi);
	eta = 1.0f / ctx->hit->mat->refractive_index;
	result.pdf = 0.0f;
	result.bsdf = vec_0();
	if (costheta(wi) < 0)
		eta = 1.0f / eta;
	if (!refract(wi, face_forward(vec_z(1.0f), wi), eta, &result.wo)
		|| vec_dot(ctx->gn, ctx->wi) * vec_dot(ctx->gn, result.wo) < 0)
		return (result);
	result.pdf = 1.0f;
	fresnel = f_dielectric(costheta(result.wo), 1.0f,
			ctx->hit->mat->refractive_index);
	result.bsdf = vec_mul(filter_sample(ctx->ctx->world, bxdf->base.tex,
				ctx->hit->hit.uv), vec_sub(vec3(1.0f, 1.0f, 1.0f),
				vec3(fresnel, fresnel, fresnel)));
	result.bsdf = vec_scale(result.bsdf, (eta) * (1.0f / eta));
	result.bsdf = vec_scale(result.bsdf, 1.0f
			/ rt_abs(costheta(result.wo)));
	return (result);
}

t_vec
	transmissive_f(const t_bxdf_ctx *ctx,
			const GLOBAL t_bxdf_transmissive *bxdf, t_vec wo)
{
	(void) ctx;
	(void) bxdf;
	(void) wo;
	return (vec_0());
}

float
	transmissive_pdf(const t_bxdf_ctx *ctx,
			const GLOBAL t_bxdf_transmissive *bxdf, t_vec wo)
{
	(void) ctx;
	(void) bxdf;
	(void) wo;
	return (0.0f);
}
