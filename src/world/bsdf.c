#include "world.h"

t_vec world_to_local(t_world_hit hit, t_vec v) {
	t_vec	ns;
	t_vec	ss;
	t_vec	ts;

	ns = hit.relative_normal;
	ss = vec_norm(hit.hit.dpdu);
	ts = vec_cross(ns, ss);
	return (vec(vec_dot(v, ss),
				vec_dot(v, ts),
				vec_dot(v, ns),
				0.0));
	return vec_norm(v);
}

t_vec local_to_world(t_world_hit hit, t_vec v) {
	t_vec	ns;
	t_vec	ss;
	t_vec	ts;

	ns = hit.relative_normal;
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
	color = filter_sample(world, bxdf->base.tex, hit.hit.uv);
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
	d_beckman_distribution(t_vec wh, t_vec n, FLOAT roughness)
{
	FLOAT	whn;
	FLOAT	dividend;
	FLOAT	divisor;
	FLOAT	m;

	whn = vec_dot(wh, n);
	m = rt_sqrt(roughness);
	divisor = 4.0 * m * m * rt_pow(whn, 4.0);
	if (divisor == 0.0)
		return (0.0);
	divisor = 1.0 / divisor;
	dividend = ((whn * whn) - 1.0) /
			(m * m * whn * whn);
	/*
	printf("e^%f / %f\n", dividend, divisor);
	*/
	return (rt_exp(dividend) * divisor);
}

static FLOAT
	fresnel_fac(FLOAT eta, t_vec wi, t_vec wh)
{
	FLOAT	wih;
	FLOAT	g;
	FLOAT	l;
	FLOAT	r;
	FLOAT	gmwih;
	FLOAT	gpwih;
	FLOAT	g2;

	wih = vec_dot(wi, wh);
	g2 = (eta * eta) - 1.0 + (wih * wih);
	if (g2 < 0)
		return (1.0);
	g = rt_sqrt(g2);
	gmwih = g - wih;
	gpwih = g + wih;
	l = 0.5 * (rt_pow(gmwih, 2.0) / rt_pow(gpwih, 2.0));
	r = (rt_pow((wih * gpwih) - 1.0, 2.0) / rt_pow((wih * gmwih) + 1.0, 2.0)) + 1.0;
	return (l * r);
}

static FLOAT
	geometric_attenuation(t_vec wi, t_vec wo, t_vec wh, t_vec n)
{
	FLOAT	whn;
	FLOAT	win;
	FLOAT	won;
	FLOAT	whwo;
	FLOAT	g1;
	FLOAT	g2;

	whn = vec_dot(wh, n);
	win = vec_dot(wi, n);
	won = vec_dot(wo, n);
	whwo = vec_dot(wh, wo);
	
	g1 = (2.0 * whn * win) / whwo;
	g2 = (2.0 * whn * won) / whwo;
	return (rt_min(1.0, rt_min(g1, g2)));
}

static t_vec
	f_cook_torrance_specular_part(const t_bxdf_cook_torrance *bxdf, t_world_hit hit, t_vec wi, t_vec wo)
{
	FLOAT	fresnel; /* fresnel factor. TODO make vector */
	FLOAT	d; /* distribution */
	FLOAT	g; /* geometric attenuation factor */
	t_vec	wh; /* halfway vector */
	t_vec	n; /* normal vector */
	FLOAT	result; /* TODO make result vector */

	wh = vec_add(wi, wo);
	if (vec_eq(wh, vec_0()))
		return vec_0();
	wh = vec_norm(wh);
	fresnel = fresnel_fac(bxdf->eta, wi, wh);
	n = world_to_local(hit, hit.relative_normal);
	d = d_beckman_distribution(wh, n, bxdf->roughness);
	g = geometric_attenuation(wi, wo, wh, n);
	result = fresnel * ((d * g) / (RT_PI * (vec_dot(n, wi) * vec_dot(n, wo))));
	return (vec(result, result, result, 0.0));
}

static t_vec
	f_bxdf_lambertian(const GLOBAL t_world *world, t_filter filter, t_vec2 uv)
{
	t_vec	color;

	color = filter_sample(world, filter, uv);
	return (vec_scale(color, RT_1_PI * 2));
}

static t_vec
	f_cook_torrance(const GLOBAL t_world *world, const t_bxdf_cook_torrance *bxdf, t_world_hit hit, t_vec wi, t_vec wo)
{
	t_vec	diffuse;
	t_vec	specular;
	t_vec	result;

	diffuse = f_bxdf_lambertian(world, bxdf->base.tex, hit.hit.uv);
	specular = f_cook_torrance_specular_part(bxdf, hit, wo, wi);
	result = vec_add(vec_scale(diffuse, bxdf->k), vec_scale(specular, 1.0 - bxdf->k));
	return (result);
}

static t_vec
	f_cook_torrance_sample(const GLOBAL t_world *world, GLOBAL t_context *ctx, const t_bxdf_cook_torrance *bxdf, t_world_hit hit, t_vec wi, t_vec *wo)
{
	while (1)
	{
		*wo = rt_random_on_hemi(&ctx->seed, hit.relative_normal);
		if (vec_dot(*wo, hit.geometric_normal) >= 0)
			break;
	}
	*wo = world_to_local(hit, *wo);
	*wo = vec3(-x(wi), -y(wi), z(wi)); /*  perfect reflection  */
	return (f_cook_torrance(world, bxdf, hit, wi, *wo));
}

static t_vec
	f_bxdf_perfect_reflective_sample(const GLOBAL t_world *world, GLOBAL t_context *ctx, const GLOBAL t_bxdf_reflective *bxdf, t_world_hit hit, t_vec wi, t_vec *wo)
{
	(void)ctx;
	(void) bxdf;
	*wo = vec3(-x(wi), -y(wi), z(wi)); /*  perfect reflection  */
	/**wo = vec_sub(wi, vec_scale(hit.relative_normal, 2.0 * vec_dot(local_to_world(hit, wi), hit.relative_normal)));
	*wo = vec_norm(vec_add(*wo, rt_random_in_sphere(&ctx->seed, 0.0, bxdf->fuzzy)));*/
	return (filter_sample(world, bxdf->base.tex, hit.hit.uv));
}

static t_vec
	f_bxdf_diffuse_sample(const GLOBAL t_world *world, GLOBAL t_context *ctx, const GLOBAL t_bxdf_diffuse *bxdf, t_world_hit hit, t_vec wi, t_vec *wo)
{
	while (1)
	{
		*wo = rt_random_on_hemi(&ctx->seed, hit.relative_normal);
		if (vec_dot(*wo, hit.geometric_normal) >= 0)
			break;
	}
	*wo = world_to_local(hit, *wo);
	return (f_bxdf_diffuse(world, bxdf, hit, wi, *wo));
}

static t_vec
	refract(t_vec wi, t_vec n, FLOAT etai_etat)
{
	FLOAT	costheta;
	FLOAT	tmp;
	t_vec	rperp;
	t_vec	rparl;

	costheta = rt_min(vec_dot(vec_neg(wi), n), 1.0);
	rperp = vec_scale(vec_add(vec_scale(n, costheta), wi), etai_etat);
	tmp = -rt_sqrt(rt_abs(1.0 - vec_mag2(rperp)));
	rparl = vec_scale(n, tmp);
	return (vec_add(rperp, rparl));
}

static FLOAT
	reflectance(FLOAT costheta, FLOAT etai_etat)
{
	FLOAT	r0;
	
	r0 = (1.0 - etai_etat) / (1.0 + etai_etat);
	r0 = r0 * r0;
	return (r0 + (1.0 - r0) * rt_pow(1.0 - costheta, 5.0));
}

static void
	push_eta(t_trace_ctx *trace_ctx, FLOAT eta)
{
	if (trace_ctx->eta_size == RT_MAX_ETA - 1)
		return;
	trace_ctx->eta[trace_ctx->eta_size] = eta;
	trace_ctx->eta_size += 1;
}

static void
	pop_eta(t_trace_ctx *trace_ctx)
{

}

static FLOAT
	top_eta(t_trace_ctx *trace_ctx)
{

}

static t_vec f_bxdf_transmissive_sample(const GLOBAL t_world *world, GLOBAL t_context *ctx, t_trace_ctx *trace_ctx,
					  const GLOBAL t_bxdf_transmissive *bxdf, t_world_hit hit, t_vec wiw, t_vec *wow)
{
	t_vec	color;
	FLOAT	etai_etat;
	FLOAT	costheta;
	FLOAT	sintheta;

	etai_etat = bxdf->eta;
	if (vec_dot(hit.relative_normal, hit.hit.normal) > 0.0)
		etai_etat = 1.0 / bxdf->eta;
	costheta = rt_min(vec_dot(vec_neg(wiw), hit.relative_normal), 1.0);
	sintheta = rt_sqrt(1.0 - costheta*costheta);
	color = filter_sample(world, bxdf->base.tex, hit.hit.uv);
	if (reflectance(costheta, etai_etat) > rt_random_float(&ctx->seed) || etai_etat * sintheta > 1.0)
	{
		*wow = vec_sub(wiw, vec_scale(hit.relative_normal, 2.0 * vec_dot(wiw, hit.relative_normal)));
	}
	else
	{
		*wow = vec_norm(refract(wiw, hit.relative_normal, etai_etat));
	}
	return (color);
}

static t_vec
	f_bxdf_sample(const GLOBAL t_world *world, GLOBAL t_context *ctx, t_trace_ctx *trace_ctx, const GLOBAL t_bxdf *bxdf, t_world_hit hit, t_vec wiw, t_vec *wow)
{
	t_vec	wi;
	t_vec	wo;
	t_vec	result;

	wi = world_to_local(hit, wiw);
	result = vec_0();
	wo = vec_z(1.0);
	if (bxdf->type == RT_BXDF_DIFFUSE)
		result = f_bxdf_diffuse_sample(world, ctx, (const GLOBAL t_bxdf_diffuse *) bxdf, hit, wi, &wo);
	if (bxdf->type == RT_BXDF_REFLECTIVE)
		result = f_bxdf_perfect_reflective_sample(world, ctx, (const GLOBAL t_bxdf_reflective *) bxdf, hit, wi, &wo);
	if (bxdf->type == RT_BXDF_COOK_TORRANCE)
		result = f_cook_torrance_sample(world, ctx, (const GLOBAL t_bxdf_cook_torrance *) bxdf, hit, wi, &wo);
	if (bxdf->type == RT_BXDF_TRANSMISSIVE)
	{
		return (f_bxdf_transmissive_sample(world, ctx, trace_ctx, (const GLOBAL t_bxdf_transmissive *) bxdf, hit, wiw, wow));;
	}
	*wow = local_to_world(hit, wo);
	return result;
}

t_vec
	f_bsdf_sample(const GLOBAL t_world *world, GLOBAL t_context *ctx, t_trace_ctx *trace_ctx, t_bsdf bsdf, t_world_hit hit, t_vec wiw, t_vec *wow)
{
	uint32_t			idx;
	FLOAT				rand;
	const GLOBAL t_bxdf	*bxdf;

	idx = bsdf.begin;
	rand = rt_random_float_range(&ctx->seed, 0.0, bsdf.weight);
	while (idx < bsdf.end)
	{
		bxdf = get_bxdf_const(world, idx);
		if (bxdf->weight != 0.0)
		{
			rand -= bxdf->weight;
			if (rand <= 0)
				return (vec_scale(f_bxdf_sample(world, ctx, trace_ctx, bxdf, hit, wiw, wow), bsdf.weight));
		}
		idx++;
	}
	return (vec(0, 0, 0, 0));
}
