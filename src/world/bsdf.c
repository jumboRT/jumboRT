#include "world.h"

static inline t_vec world_to_local(t_world_hit hit, t_vec v) {
	/*
	t_vec		ns;
	t_vec		ss;
	t_vec		ts;

	ns = hit.normal; This is probably not correct, called 'shading normal' in pbrt
	ss = vec_norm(hit.dpdu);
	ts = vec_cross(ns ,ss);
	return (vec(vec_dot(v, ss),
				vec_dot(v, ts),
				vec_dot(v, ns),
				0.0));
	*/
	(void) hit;
	return v;
}

static inline t_vec local_to_world(t_world_hit hit, t_vec v) {
	(void) hit;
	return v;
}

t_vec
	f_bxdf_diffuse(const GLOBAL t_world *world, const t_bxdf_diffuse *bxdf, t_world_hit hit, t_vec wi, t_vec wo)
{
	t_vec	color;

	(void) wi;
	(void) wo;
	color = tex_sample_id(world, bxdf->tex, hit.hit.uv);
	return (vec_scale(color, RT_1_PI * 2));
}

t_vec
	f_bxdf_perfect_reflective(const GLOBAL t_world *world, const t_bxdf_reflective *bxdf, t_world_hit hit, t_vec wi, t_vec wo)
{
	(void) world;
	(void) hit;
	(void) bxdf;
	(void) wi;
	(void) wo;
	return (vec(0.0, 0.0, 0.0, 0.0));
}

t_vec
	f_bxdf_perfect_reflective_sample(const GLOBAL t_world *world, GLOBAL t_context *ctx, const t_bxdf_reflective *bxdf, t_world_hit hit, t_vec wi, t_vec *wo)
{
	(void)ctx;
	(void) bxdf;
	/**wo = vec_neg(wi);  perfect reflection */
	*wo = vec_sub(wi, vec_scale(hit.relative_normal, 2.0 * vec_dot(wi, hit.relative_normal)));
	*wo = vec_norm(vec_add(*wo, rt_random_in_sphere(&ctx->seed, 0.0, bxdf->fuzzy)));
	return (tex_sample_id(world, bxdf->tex, hit.hit.uv));
}

t_vec
	f_bxdf_refractive(const GLOBAL t_world *world, const t_bxdf_refractive *bxdf, t_world_hit hit, t_vec wi, t_vec wo)
{
	(void) world;
	(void) bxdf;
	(void) hit;
	(void) wi;
	(void) wo;
	return (vec(1.0, 0.0, 1.0, 0.0));
}

t_vec
	f_bxdf(const GLOBAL  t_world *world, const t_bxdf *bxdf, t_world_hit hit, t_vec wi, t_vec wo)
{
	if (bxdf->type == RT_BXDF_DIFFUSE) {
		return f_bxdf_diffuse(world, (const t_bxdf_diffuse *) bxdf, hit, wi, wo);
	}
	if (bxdf->type == RT_BXDF_REFLECTIVE) {
		return f_bxdf_perfect_reflective(world, (const t_bxdf_reflective *) bxdf, hit, wi, wo);
	}
	if (bxdf->type == RT_BXDF_REFRACTIVE) {
		return f_bxdf_refractive(world, (const t_bxdf_refractive *) bxdf, hit, wi, wo);
	}
	return (vec_0());
}

t_vec
	f_bsdf(const GLOBAL t_world *world, t_material mat, t_world_hit hit, t_vec wiw, t_vec wow)
{
	t_vec				wi;
	t_vec				wo;
	t_vec				ng;
	t_vec				result;
	uint32_t			index;
	int32_t				reflect;
	const t_bxdf		*bxdf;

	ng = hit.hit.normal;
	wi = world_to_local(hit, wiw);
	wo = world_to_local(hit, wow);
	reflect = (vec_dot(wiw, ng) * vec_dot(wow, ng)) <= 0;
	result = vec_0();
	index = mat.bxdf_begin;
	while (index < mat.bxdf_end) {
		bxdf = get_bxdf_const(world, index);
		if ((reflect && (bxdf->type == RT_BXDF_DIFFUSE || bxdf->type == RT_BXDF_REFLECTIVE))
					|| (!reflect && (bxdf->type == RT_BXDF_REFRACTIVE))) {
			result = vec_add(result, f_bxdf(world, bxdf, hit, wi, wo));
		}
		index += 1;
	}
	return (result);
}

t_vec
	f_bxdf_diffuse_sample(const GLOBAL t_world *world, GLOBAL t_context *ctx, const t_bxdf_diffuse *bxdf, t_world_hit hit, t_vec wi, t_vec *wo)
{
	*wo = rt_random_on_hemi(&ctx->seed, hit.relative_normal);
	return (f_bxdf_diffuse(world, bxdf, hit, wi, *wo));
}

t_vec
	f_bxdf_sample(const GLOBAL t_world *world, GLOBAL t_context *ctx, const t_bxdf *bxdf, t_world_hit hit, t_vec wi, t_vec *wo)
{
	if (bxdf->type == RT_BXDF_DIFFUSE) {
		return (f_bxdf_diffuse_sample(world, ctx, (const t_bxdf_diffuse *) bxdf, hit, wi, wo));
	}
	if (bxdf->type == RT_BXDF_REFLECTIVE) {
		return (f_bxdf_perfect_reflective_sample(world, ctx, (const t_bxdf_reflective *) bxdf, hit, wi, wo));
	}
	return vec_0();
}

int
	f_bsdf_sample(const GLOBAL t_world *world, GLOBAL t_context *ctx, t_material mat, t_world_hit hit, t_vec wiw, t_vec *wow, t_vec *result)
{
	uint32_t		count;
	uint32_t		idx;
	t_vec			wi;
	t_vec			wo;
	const t_bxdf	*bxdf;

	wi = world_to_local(hit, wiw);
	count = mat.bxdf_end - mat.bxdf_begin;
	if (count == 0)
	{
		return (0);
	}
	idx = rt_random(&ctx->seed) % count;
	bxdf = get_bxdf_const(world, mat.bxdf_begin + idx);
	*result = f_bxdf_sample(world, ctx, bxdf, hit, wi, &wo);
	*wow = local_to_world(hit, wo);
	return (1);
}
