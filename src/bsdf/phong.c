#include "bsdf.h"
#include "world.h"

static t_vec
	wtol(t_vec n0, t_vec v)
{
	t_vec	ns;
	t_vec	ss;
	t_vec	ts;

	ns = n0;
	ss = vec_norm(vec_cross(n0, vec3(0.0f, 1.0f, 0.0f)));
	ts = vec_norm(vec_cross(n0, vec3(1.0f, 0.0f, 0.0f)));
	return (vec(vec_dot(v, ss),
			vec_dot(v, ts),
			vec_dot(v, ns),
			0.0));
}

static t_vec
	ltow(t_vec n0, t_vec v)
{
	t_vec	ns;
	t_vec	ss;
	t_vec	ts;

	ns = n0;
	ss = vec_norm(vec_cross(n0, vec3(0.0f, 1.0f, 0.0f)));
	ts = vec_norm(vec_cross(n0, vec3(1.0f, 0.0f, 0.0f)));
	return (vec_norm2(vec(
				x(ss) * x(v) + x(ts) * y(v) + x(ns) * z(v),
				y(ss) * x(v) + y(ts) * y(v) + y(ns) * z(v),
				z(ss) * x(v) + z(ts) * y(v) + z(ns) * z(v),
				0.0)));
}

t_sample
	phong_sample(const t_bxdf_ctx *ctx,
			const GLOBAL t_bxdf_phong *bxdf)
{
	t_sample	result;
	float		epsilon[2];
	float		theta;
	float		phi;
	float		roughness;

	result.wo = vec_set(ctx->wi, 2, -z(ctx->wi));
	epsilon[0] = rt_random_float(&ctx->ctx->ctx->seed);
	epsilon[1] = rt_random_float(&ctx->ctx->ctx->seed);
	roughness = w(filter_sample(ctx->ctx->world,
				bxdf->roughness, ctx->hit->hit.uv));
	theta = rt_acos(rt_pow(epsilon[0], 1.0f / roughness));
	phi = RT_2PI * epsilon[1];
	result.wo = ltow(result.wo, sphere_to_cart(theta, phi));
	result.pdf = phong_pdf(ctx, bxdf, result.wo);
	result.bsdf = phong_f(ctx, bxdf, result.wo);
	return (result);
}

t_vec
	phong_f(const t_bxdf_ctx *ctx,
			const GLOBAL t_bxdf_phong *bxdf, t_vec wow)
{
	t_vec	reflect;
	t_vec	wo;
	t_vec	color;
	float	roughness;

	if (same_hemi(ctx->wi, wow))
		return (vec_0());
	reflect = vec_set(ctx->wi, 2, -z(ctx->wi));
	wo = wtol(reflect, wow);
	roughness = w(filter_sample(ctx->ctx->world, bxdf->roughness,
				ctx->hit->hit.uv));
	color = filter_sample(ctx->ctx->world, bxdf->base.tex, ctx->hit->hit.uv);
	return (vec_mul(vec_scale(vec_scale(vec3(1.0f, 1.0f, 1.0f),
					(((roughness + 2) / RT_2PI)
						* rt_pow(costheta(wo), roughness))),
				1.0 / rt_abs(costheta(wow))), color));
}

float
	phong_pdf(const t_bxdf_ctx *ctx,
			const GLOBAL t_bxdf_phong *bxdf, t_vec wow)
{
	t_vec	reflect;
	t_vec	wo;
	float	roughness;

	if (same_hemi(ctx->wi, wow))
		return (0.0f);
	reflect = vec_set(ctx->wi, 2, -z(ctx->wi));
	wo = wtol(reflect, wow);
	roughness = w(filter_sample(ctx->ctx->world, bxdf->roughness,
				ctx->hit->hit.uv));
	return (((roughness + 1) / (RT_2PI)) * rt_pow(costheta(wo), roughness));
}
