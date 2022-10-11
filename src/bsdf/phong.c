#include "bsdf.h"
#include "world.h"

static t_vec wtol(t_vec n0, t_vec n1, t_vec n2, t_vec v) {
	t_vec	ns;
	t_vec	ss;
	t_vec	ts;

	ns = n0;
	ss = n1;
	ts = n2;
	return (vec(vec_dot(v, ss),
				vec_dot(v, ts),
				vec_dot(v, ns),
				0.0));
}

static t_vec ltow(t_vec n0, t_vec n1, t_vec n2, t_vec v) {
	t_vec	ns;
	t_vec	ss;
	t_vec	ts;

	ns = n0;
	ss = n1;
	ts = n2;
	return (vec_norm2(vec(
			x(ss) * x(v) + x(ts) * y(v) + x(ns) * z(v),
			y(ss) * x(v) + y(ts) * y(v) + y(ns) * z(v),
			z(ss) * x(v) + z(ts) * y(v) + z(ns) * z(v),
			0.0)));
}

t_sample
	phong_sample(t_trace_ctx *ctx, const t_world_hit *hit,
			const GLOBAL t_bxdf_phong *bxdf, t_vec wiw)
{
	t_sample	result;
	float		epsilon[2];
	float		theta;
	float		phi;
	t_vec		tmp;

	result.wo = vec_set(wiw, 2, -z(wiw));
	epsilon[0] = rt_random_float(&ctx->ctx->seed);
	epsilon[1] = rt_random_float(&ctx->ctx->seed);
	theta = rt_acos(rt_pow(epsilon[0], 1.0f / (w(filter_sample(ctx->world,
						bxdf->base.tex, hit->hit.uv)))));
	phi = RT_2PI * epsilon[1];
	tmp = sphere_to_cart(theta, phi);
//	result.wo = ltow(result.wo, vec_norm(vec_cross(result.wo, hit->hit.dpdu)),
//						vec_norm(vec_cross(result.wo, hit->hit.dpdv)), tmp);
	result.bsdf = phong_f(ctx, hit, bxdf, wiw, result.wo);
	result.pdf = phong_pdf(ctx, hit, bxdf, wiw, result.wo);
	return (result);
}

t_vec
	phong_f(t_trace_ctx *ctx, const t_world_hit *hit,
			const GLOBAL t_bxdf_phong *bxdf, t_vec wiw, t_vec wow)
{
	t_vec	reflect;
	t_vec	wo;
	float	roughness;

	return (vec3(1.0f, 1.0f, 1.0f));
	reflect = vec_set(wiw, 2, -z(wiw));
	wo = wtol(reflect, vec_norm(vec_cross(reflect, hit->hit.dpdu)),
						vec_norm(vec_cross(reflect, hit->hit.dpdv)), wow);
	roughness = w(filter_sample(ctx->world, bxdf->base.tex, hit->hit.uv));
	return (vec_scale(vec3(1.0f, 1.0f, 1.0f),
			(((roughness + 2) / RT_2PI) * rt_pow(costheta(wo), roughness))));
}

float
	phong_pdf(t_trace_ctx *ctx, const t_world_hit *hit,
			const GLOBAL t_bxdf_phong *bxdf, t_vec wiw, t_vec wow)
{
	t_vec	reflect;
	t_vec	wo;
	float	roughness;

	return (1.0f);
	reflect = vec_set(wiw, 2, -z(wiw));
	wo = wtol(reflect, vec_norm(vec_cross(reflect, hit->hit.dpdu)),
						vec_norm(vec_cross(reflect, hit->hit.dpdv)), wow);
	roughness = w(filter_sample(ctx->world, bxdf->base.tex, hit->hit.uv));
	return (((roughness + 1) / (RT_2PI)) * rt_pow(costheta(wo), roughness));
}
