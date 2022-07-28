#include "world.h"

static inline t_vec world_to_local(t_hit hit, t_vec v) {
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

t_vec
	f_bxdf_diffuse(const GLOBAL t_world *world, t_material mat, t_hit hit, const t_bxdf_diffuse *bxdf, t_vec wi, t_vec wo)
{
	t_vec	color;

	(void) wi;
	(void) wo;
	(void) mat;
	color = tex_sample_id(world, bxdf->tex, hit.uv);
	return (vec_scale(color, RT_1_PI));
}

t_vec
	f_bxdf_reflective(const GLOBAL t_world *world, t_material mat, t_hit hit, const t_bxdf_reflective *bxdf, t_vec wi, t_vec wo)
{
	(void) world;
	(void) bxdf;
	(void) mat;
	(void) hit;
	(void) wi;
	(void) wo;
	return (vec(1.0, 0.0, 1.0, 0.0));
}

t_vec
	f_bxdf_refractive(const GLOBAL t_world *world, t_material mat, t_hit hit, const t_bxdf_refractive *bxdf, t_vec wi, t_vec wo)
{
	(void) world;
	(void) bxdf;
	(void) mat;
	(void) hit;
	(void) wi;
	(void) wo;
	return (vec(1.0, 0.0, 1.0, 0.0));
}

t_vec
	f_bxdf(const GLOBAL  t_world *world, t_material mat, t_hit hit, const t_bxdf *bxdf, t_vec wi, t_vec wo)
{
	if (bxdf->type == RT_BXDF_DIFFUSE) {
		return f_bxdf_diffuse(world, mat, hit, (const t_bxdf_diffuse *) bxdf, wi, wo);
	}
	if (bxdf->type == RT_BXDF_REFLECTIVE) {
		return f_bxdf_reflective(world, mat, hit, (const t_bxdf_reflective *) bxdf, wi, wo);
	}
	if (bxdf->type == RT_BXDF_REFRACTIVE) {
		return f_bxdf_refractive(world, mat, hit, (const t_bxdf_refractive *) bxdf, wi, wo);
	}
	return (vec_0());
}

t_vec
	f_bsdf(const GLOBAL t_world *world, t_material mat, t_hit hit, t_vec wiw, t_vec wow)
{
	t_vec				wi;
	t_vec				wo;
	t_vec				ng;
	t_vec				result;
	uint32_t			index;
	int32_t				reflect;
	const t_bxdf		*bxdf;

	ng = hit.normal;
	wi = world_to_local(hit, wiw);
	wo = world_to_local(hit, wow);
	reflect = (vec_dot(wiw, ng) * vec_dot(wow, ng)) <= 0;
	result = vec_0();
	index = mat.bxdf_begin;
	while (index < mat.bxdf_end) {
		bxdf = get_bxdf_const(world, index);
		if ((reflect && (bxdf->type == RT_BXDF_DIFFUSE || bxdf->type == RT_BXDF_REFLECTIVE))
					|| (!reflect && (bxdf->type == RT_BXDF_REFRACTIVE))) {
			result = vec_add(result, f_bxdf(world, mat, hit, bxdf, wi, wo));
		}
		index += world_bxdf_size(bxdf->type) / RT_BXDF_ALIGN;
	}
	return (result);
}
