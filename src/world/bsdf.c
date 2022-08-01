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

static t_vec
	f_bxdf_diffuse(const GLOBAL t_world *world, const GLOBAL t_bxdf_diffuse *bxdf, t_world_hit hit, t_vec wi, t_vec wo)
{
	t_vec	color;

	(void) wi;
	(void) wo;
	color = tex_sample_id(world, bxdf->base.tex, hit.hit.uv);
	return (vec_scale(color, RT_1_PI * 2));
}

static t_vec
	f_bxdf_perfect_reflective(const GLOBAL t_world *world, const GLOBAL t_bxdf_reflective *bxdf, t_world_hit hit, t_vec wi, t_vec wo)
{
	(void) world;
	(void) hit;
	(void) bxdf;
	(void) wi;
	(void) wo;
	return (vec(0.0, 0.0, 0.0, 0.0));
}

static t_vec
	f_bxdf_perfect_reflective_sample(const GLOBAL t_world *world, GLOBAL t_context *ctx, const GLOBAL t_bxdf_reflective *bxdf, t_world_hit hit, t_vec wi, t_vec *wo)
{
	(void)ctx;
	(void) bxdf;
	/**wo = vec_neg(wi);  perfect reflection */
	*wo = vec_sub(wi, vec_scale(hit.relative_normal, 2.0 * vec_dot(wi, hit.relative_normal)));
	*wo = vec_norm(vec_add(*wo, rt_random_in_sphere(&ctx->seed, 0.0, bxdf->fuzzy)));
	return (tex_sample_id(world, bxdf->base.tex, hit.hit.uv));
}

static t_vec
	f_bxdf_refractive(const GLOBAL t_world *world, const GLOBAL t_bxdf_refractive *bxdf, t_world_hit hit, t_vec wi, t_vec wo)
{
	(void) world;
	(void) bxdf;
	(void) hit;
	(void) wi;
	(void) wo;
	return (vec(1.0, 0.0, 1.0, 0.0));
}

static t_vec
	f_bxdf_diffuse_sample(const GLOBAL t_world *world, GLOBAL t_context *ctx, const GLOBAL t_bxdf_diffuse *bxdf, t_world_hit hit, t_vec wi, t_vec *wo)
{
	*wo = rt_random_on_hemi(&ctx->seed, hit.relative_normal);
	return (f_bxdf_diffuse(world, bxdf, hit, wi, *wo));
}

static t_vec
	f_bxdf_sample(const GLOBAL t_world *world, GLOBAL t_context *ctx, const GLOBAL t_bxdf *bxdf, t_world_hit hit, t_vec wiw, t_vec *wow)
{
	t_vec	wi;
	t_vec	wo;
	t_vec	result;

	wi = world_to_local(hit, wiw);
	result = vec_0();
	if (bxdf->type == RT_BXDF_DIFFUSE)
		result = f_bxdf_diffuse_sample(world, ctx, (const GLOBAL t_bxdf_diffuse *) bxdf, hit, wi, &wo);
	if (bxdf->type == RT_BXDF_REFLECTIVE)
		result = f_bxdf_perfect_reflective_sample(world, ctx, (const GLOBAL t_bxdf_reflective *) bxdf, hit, wi, &wo);
	*wow = local_to_world(hit, wo);
	return result;
}

static t_vec
	bxdf_color(const GLOBAL t_world *world, const GLOBAL t_bxdf *bxdf, t_vec2 uv)
{
	return (tex_sample_id(world, bxdf->tex, uv));
}

static FLOAT
	bxdf_weight(const GLOBAL t_world *world, const GLOBAL t_bxdf *bxdf, t_vec color, t_vec2 uv)
{
	return (vec_dot(color, bxdf_color(world, bxdf, uv)));
}

static FLOAT
	bsdf_total_weight(const GLOBAL t_world *world, t_material mat, t_world_hit hit, t_vec color)
{
	uint32_t			idx;
	FLOAT				result;
	const GLOBAL t_bxdf	*bxdf;

	result = 0.0;
	idx = mat.bxdf_begin;
	while (idx < mat.bxdf_end) {
		bxdf = get_bxdf_const(world, idx);
		result += bxdf_weight(world, bxdf, color, hit.hit.uv);
		idx += 1;
	}
	return (result);
}

int
	f_bsdf_sample(const GLOBAL t_world *world, GLOBAL t_context *ctx, t_material mat, t_world_hit hit, t_vec wiw, t_vec color, t_vec *wow, t_vec *result)
{
	uint32_t			idx;
	FLOAT				total_weight;
	FLOAT				rand;
	FLOAT				weight;
	const GLOBAL t_bxdf	*bxdf;

	total_weight = bsdf_total_weight(world, mat, hit, color);
	*result = vec_0();
	idx = mat.bxdf_begin;
	rand = rt_random_float_range(&ctx->seed, 0.0, total_weight);
	while (idx < mat.bxdf_end)
	{
		bxdf = get_bxdf_const(world, idx);
		weight = bxdf_weight(world, bxdf, color, hit.hit.uv);
		if (weight != 0.0)
		{
			rand -= weight;
			if (rand <= 0)
			{
				*result = vec_scale(f_bxdf_sample(world, ctx, bxdf, hit, wiw, wow), (weight) / total_weight);
				return (1);
			}
		}
		idx++;
	}
	return (0);
}
