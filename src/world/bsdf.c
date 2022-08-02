#include "world.h"

static inline t_vec world_to_local(t_world_hit hit, t_vec v) {
	t_vec	ns;
	t_vec	ss;
	t_vec	ts;

	ns = hit.relative_normal; /* This is probably not correct, called 'shading normal' in pbrt */
	ss = vec_norm(hit.hit.dpdu);
	ts = vec_cross(ns, ss);
	return (vec(vec_dot(v, ss),
				vec_dot(v, ts),
				vec_dot(v, ns),
				0.0));
	return vec_norm(v);
}

static inline t_vec local_to_world(t_world_hit hit, t_vec v) {
	t_vec	ns;
	t_vec	ss;
	t_vec	ts;

	ns = hit.relative_normal; /* This is probably not correct, called 'shading normal' in pbrt */
	ss = vec_norm(hit.hit.dpdu);
	ts = vec_cross(ns, ss);
	return (vec_norm(vec(
			x(ss) * x(v) + x(ts) * y(v) + x(ns) * z(v),
			y(ss) * x(v) + y(ts) * y(v) + y(ns) * z(v),
			z(ss) * x(v) + z(ts) * y(v) + z(ns) * z(v),
			0.0)));
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

static inline FLOAT
	costheta(t_vec w)
{
	return (z(w));
}

static inline FLOAT
	cos2theta(t_vec w)
{
	return (z(w) * z(w));
}

static inline FLOAT
	abs_costheta(t_vec w)
{
	return (rt_abs(z(w)));
}

static inline FLOAT
	sin2theta(t_vec w)
{
	return (rt_max(0.0, 1.0 - cos2theta(w)));
}

static inline FLOAT
	sintheta(t_vec w)
{
	return (rt_sqrt(sin2theta(w)));
}

static inline FLOAT
	tantheta(t_vec w)
{
	return (sintheta(w) / costheta(w));
}

static inline FLOAT
	tan2theta(t_vec w)
{
	return (sin2theta(w) / cos2theta(w));
}

static inline FLOAT
	cosphi(t_vec w)
{
	FLOAT tmp;

	tmp = sintheta(w);
	if (tmp == 0.0)
		return (1.0);
	return (rt_clamp(x(w) / tmp, -1.0, 1.0));
}

static inline FLOAT
	sinphi(t_vec w)
{
	FLOAT tmp;

	tmp = sintheta(w);
	if (tmp == 0.0)
		return (0.0);
	return (rt_clamp(y(w) / tmp, -1.0, 1.0));
}

static inline FLOAT
	cos2phi(t_vec w)
{
	return (cosphi(w) * cosphi(w));
}

static inline FLOAT
	sin2phi(t_vec w)
{
	return (sinphi(w) * sinphi(w));
}

static FLOAT
	beckmann_lambda(const t_bxdf_mf_reflection *bxdf, t_vec w)
{
	FLOAT	abs_tantheta;
	FLOAT	alpha;
	FLOAT	a;

	abs_tantheta = rt_abs(tantheta(w));
	if (abs_tantheta >= RT_HUGE_VAL)
		return (0.0);
	alpha = rt_sqrt(cos2phi(w) * bxdf->alphax * bxdf->alphax + sin2phi(w) * bxdf->alphay * bxdf->alphay);
	a = 1.0 / (alpha * abs_tantheta);
	if (a >= 1.6)
		return (0.0);
	return ((1.0 - 1.259 * a + 0.396 * a * a) /
		(3.535 * a + 2.181 * a * a));
}

static FLOAT
	trowbridge_lambda(const t_bxdf_mf_reflection *bxdf, t_vec w)
{
	FLOAT	abs_tan;
	FLOAT	alpha;
	FLOAT	alpha2tan2;

	abs_tan = rt_abs(tantheta(w));
	if (abs_tan >= RT_HUGE_VAL)
		return (0.0);
	alpha = rt_sqrt(cos2phi(w) * bxdf->alphax * bxdf->alphax +
			sin2phi(w) * bxdf->alphay * bxdf->alphay);
	alpha2tan2 = (alpha * abs_tan) * (alpha * abs_tan);
	return ((-1.0 + rt_sqrt(1.0 + alpha2tan2)) / 2.0);
}

static FLOAT
	mf_beckmann_distribution_g(const t_bxdf_mf_reflection *bxdf, t_vec wo, t_vec wi)
{
	return (1.0 / (1.0 + beckmann_lambda(bxdf, wo) + beckmann_lambda(bxdf, wi)));
}

static FLOAT
	mf_trowbridge_distribution_g(const t_bxdf_mf_reflection *bxdf, t_vec wo, t_vec wi)
{
	return (1.0 / (1.0 + trowbridge_lambda(bxdf, wo) + trowbridge_lambda(bxdf, wi)));
}

static FLOAT
	mf_beckmann_distribution_d(const t_bxdf_mf_reflection *bxdf, t_vec wh)
{
	FLOAT	tan2;
	FLOAT	cos4;
	FLOAT	result;
	FLOAT	quot;
	FLOAT	delim;
	FLOAT	inner;

	tan2 = tan2theta(wh);
	if (tan2 >= RT_HUGE_VAL)
		return (0.0);
	cos4 = cos2theta(wh);
	cos4 *= cos4;
	inner = -tan2 * (cos2phi(wh) / (bxdf->alphax * bxdf->alphax) +
							sin2phi(wh) / (bxdf->alphay * bxdf->alphay));
	quot = (rt_exp(inner));
	delim = (RT_PI * bxdf->alphax * bxdf->alphay * cos4);
	result = quot / delim;
	/*printf("tan2 %f cos4 %f cos2phi %f alphax %f sin2phi %f alphay %f inner %f quot %f delim %f result %f\n", tan2, cos4, cos2phi(wh), bxdf->alphax, sin2phi(wh), bxdf->alphay, inner, quot, delim, result);*/
	return result;
}

static FLOAT
	mf_trowbridge_distribution_d(const t_bxdf_mf_reflection *bxdf, t_vec wh)
{
	FLOAT	tan2;
	FLOAT	cos4;
	FLOAT	e;

	tan2 = tan2theta(wh);
	if (tan2 >= RT_HUGE_VAL)
		return (0.0);
	cos4 = cos2theta(wh) * cos2theta(wh);
	e = (cos2phi(wh) / (bxdf->alphax * bxdf->alphax) +
			sin2phi(wh) / (bxdf->alphay * bxdf->alphay)) * tan2;
	return (1.0 / (RT_PI * bxdf->alphax * bxdf->alphay * cos4 * (1 + e) * (1 + e)));
}

static t_vec
	f_bxdf_microfacet_reflection(const GLOBAL t_world *world, const t_bxdf_mf_reflection *bxdf, t_world_hit hit, t_vec wi, t_vec wo)
{
	FLOAT	costhetaO;
	FLOAT	costhetaI;
	t_vec	wh;
	t_vec	result;
	t_vec	color;
	FLOAT	tmp;
	FLOAT	d, g;

	costhetaO = abs_costheta(wo);
	costhetaI = abs_costheta(wi);
	wh = vec_add(wi, wo);
	result = vec_0();
	if (costhetaO == 0.0 || costhetaI == 0.0)
		return (result);
	if (vec_eq(wh, vec_0()))
		return (result);
	wh = vec_norm(wh);
	color = tex_sample_id(world, bxdf->base.tex, hit.hit.uv);
	d = mf_beckmann_distribution_d(bxdf, wh);
	g = mf_beckmann_distribution_g(bxdf, wo, wi);
	tmp = d * g;
	result = vec_scale(color, tmp / (4.0 * costhetaI * costhetaO));
	return (result);
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
	*wo = vec_neg(wi);  /* perfect reflection  */
	/**wo = vec_sub(wi, vec_scale(hit.relative_normal, 2.0 * vec_dot(wi, hit.relative_normal)));
	*wo = vec_norm(vec_add(*wo, rt_random_in_sphere(&ctx->seed, 0.0, bxdf->fuzzy)));*/
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
	*wo = world_to_local(hit, *wo);
	return (f_bxdf_diffuse(world, bxdf, hit, wi, *wo));
}

static t_vec
	f_bxdf_mf_reflective_sample(const GLOBAL t_world *world, GLOBAL t_context *ctx, const t_bxdf_mf_reflection *bxdf, t_world_hit hit, t_vec wi, t_vec *wo)
{
	*wo = rt_random_on_hemi(&ctx->seed, hit.relative_normal);
	*wo = world_to_local(hit, *wo);
	return (f_bxdf_microfacet_reflection(world, bxdf, hit, wi, *wo));
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
	if (bxdf->type == RT_BXDF_MF_REFLECTIVE)
		result = f_bxdf_mf_reflective_sample(world, ctx, (const GLOBAL t_bxdf_mf_reflection *) bxdf, hit, wi, &wo);
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

t_vec
	f_bsdf_sample(const GLOBAL t_world *world, GLOBAL t_context *ctx, t_material mat, t_world_hit hit, t_vec wiw, t_vec color, t_vec *wow)
{
	uint32_t			idx;
	FLOAT				total_weight;
	FLOAT				rand;
	FLOAT				weight;
	const GLOBAL t_bxdf	*bxdf;

	total_weight = bsdf_total_weight(world, mat, hit, color);
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
				return (vec_scale(f_bxdf_sample(world, ctx, bxdf, hit, wiw, wow), (weight) / total_weight));
		}
		idx++;
	}
	return (vec(0, 0, 0, 0));
}
