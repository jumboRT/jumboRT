#include "world.h"

static t_vec
	reflect(t_vec wiw, t_vec n)
{
	return (vec_sub(wiw, vec_scale(n, 2.0 * vec_dot(wiw, n))));
}

static float
	reflectance(float costheta, float etai_etat)
{
	float	r0;
	
	r0 = (1.0 - etai_etat) / (1.0 + etai_etat);
	r0 = r0 * r0;
	return (r0 + (1.0 - r0) * rt_pow(1.0 - costheta, 5.0));
}

static t_vec
	clip(t_vec v, t_vec n)
{
	float	dot;

	dot = vec_dot(v, n);
	if (dot >= 0)
		return (v);
	return (reflect(v, n));
}

static t_vec
	f_bxdf_reflective_sample(const GLOBAL t_world *world, GLOBAL t_context *ctx, const GLOBAL t_bxdf_reflective *bxdf, t_world_hit hit, t_vec wi, t_vec *wo)
{
	(void)ctx;
	(void) bxdf;
	*wo = reflect(wi, hit.rel_shading_normal);
	*wo = vec_norm(vec_add(*wo, vec_scale(rt_random_in_sphere(&ctx->seed), bxdf->fuzzy)));
	*wo = clip(*wo, hit.rel_geometric_normal);
	return (filter_sample(world, bxdf->base.tex, hit.hit.uv));
}

static t_vec
	f_bxdf_diffuse_sample(const GLOBAL t_world *world, GLOBAL t_context *ctx, const GLOBAL t_bxdf_diffuse *bxdf, t_world_hit hit, t_vec wi, t_vec *wo)
{
	(void) wi;
	*wo = rt_random_on_hemi(&ctx->seed, hit.rel_shading_normal);
	//*wo = rt_random_on_hemi_cos(&ctx->seed);
	//*wo = local_to_world(hit, *wo);
	*wo = clip(*wo, hit.rel_geometric_normal);
	return (filter_sample(world, bxdf->base.tex, hit.hit.uv));
}

static float
	f_bxdf_bphong_f(float theta, float alpha)
{
	return (((alpha + 2.0f) * rt_pow(rt_cos(theta), alpha + 1.0)) / RT_2PI);
}

static t_vec
	f_bxdf_bphong_sample(const GLOBAL t_world *world, GLOBAL t_context *ctx,
			const GLOBAL t_bxdf_bphong *bxdf, t_world_hit hit,
			t_vec wi, t_vec *wo)
{
	float	e0;
	float	e1;
	float	theta;
	float	phi;
	float	alpha;
	t_vec	half_vec;

	e0 = rt_random_float_range(&ctx->seed, 0, 1.0f);
	e1 = rt_random_float_range(&ctx->seed, 0, 1.0f);
	alpha = x(filter_sample(world, bxdf->alpha, hit.hit.uv));
	theta = rt_acos(rt_pow(e0, (1.0f / (alpha + 2.0f))));
	phi = RT_2PI * e1;
	*wo = sphere_to_cart(theta, phi);
	*wo = local_to_world(hit, *wo);
	half_vec = vec_norm(vec_add(*wo, wi));

	return (vec_add(vec_scale(filter_sample(world, bxdf->base.tex, hit.hit.uv),
					vec_dot(half_vec, hit.rel_shading_normal)),
			vec_scale(filter_sample(world, bxdf->spec, hit.hit.uv),
				f_bxdf_bphong_f(theta, alpha))));
}

static t_vec
	refract(t_vec wi, t_vec n, float etai_etat)
{
	float	costheta;
	float	tmp;
	t_vec	rperp;
	t_vec	rparl;

	costheta = rt_min(vec_dot(vec_neg(wi), n), 1.0);
	rperp = vec_scale(vec_add(vec_scale(n, costheta), wi), etai_etat);
	tmp = -rt_sqrt(rt_abs(1.0 - vec_mag2(rperp)));
	rparl = vec_scale(n, tmp);
	return (vec_add(rperp, rparl));
}

static void
	eta_del_link(t_trace_ctx *trace_ctx, int32_t idx)
{
	if (trace_ctx->etas[idx].prev >= 0)
		trace_ctx->etas[trace_ctx->etas[idx].prev].next = trace_ctx->etas[idx].next;
	if (trace_ctx->etas[idx].next >= 0)
		trace_ctx->etas[trace_ctx->etas[idx].next].prev = trace_ctx->etas[idx].prev;
}

static int32_t
	eta_top(t_trace_ctx *trace_ctx)
{
	int32_t idx;

	idx = 0;
	while (trace_ctx->etas[idx].next >= 0)
		idx = trace_ctx->etas[idx].next;
	return (idx);
}

static float
	eta_secondf(t_trace_ctx *trace_ctx)
{
	int32_t	idx;

	idx = eta_top(trace_ctx);
	if (trace_ctx->etas[idx].prev < 0)
		return (trace_ctx->etas[idx].eta);
	return (trace_ctx->etas[trace_ctx->etas[idx].prev].eta);
}

static void
	eta_del(t_trace_ctx *trace_ctx, uint32_t mat)
{
	int32_t idx;

	idx = eta_top(trace_ctx);
	while (idx >= 0 && trace_ctx->etas[idx].mat != mat)
		idx = trace_ctx->etas[idx].prev;
	if (idx < 0)
		return;
	eta_del_link(trace_ctx, idx);
}	

static int
	eta_has(t_trace_ctx *trace_ctx, uint32_t mat)
{
	int32_t	idx;

	idx = 0;
	while (idx >= 0)
	{
		if (trace_ctx->etas[idx].mat == mat)
			return (1);
		idx = trace_ctx->etas[idx].next;
	}
	return (0);
}

static int32_t
	eta_get(t_trace_ctx *trace_ctx, uint32_t mat)
{
	int32_t	idx;

	idx = 0;
	while (idx >= 0)
	{
		if (trace_ctx->etas[idx].mat == mat)
			break;
		idx = trace_ctx->etas[idx].next;
	}
	return (idx);
}

static void
	eta_push(t_trace_ctx *trace_ctx, uint32_t mat, float eta, uint32_t bxdf_idx)
{
	int32_t idx;

	idx = eta_top(trace_ctx);
	if (idx + 1 >= RT_MAX_ETA)
		return;
	trace_ctx->etas[idx].next = idx + 1;
	trace_ctx->etas[idx + 1].prev = idx;
	trace_ctx->etas[idx + 1].next = -1;
	trace_ctx->etas[idx + 1].mat = mat;
	trace_ctx->etas[idx + 1].eta = eta;
	trace_ctx->etas[idx + 1].bxdf = bxdf_idx;
}

void
	eta_init(t_trace_ctx *trace_ctx, float eta)
{
	trace_ctx->etas[0].prev = -1;
	trace_ctx->etas[0].next = -1;
	trace_ctx->etas[0].mat = -1;
	trace_ctx->etas[0].eta = eta;
	trace_ctx->etas[0].bxdf = -1;
}
	
static t_vec f_bxdf_transmissive_sample(const GLOBAL t_world *world, GLOBAL t_context *ctx, t_trace_ctx *trace_ctx,
					  const GLOBAL t_bxdf_transmissive *bxdf, t_world_hit hit, t_vec wiw, t_vec *wow)
{
	float	etai_etat;
	float	costheta;
	float	sintheta;
	int		entering;
	float	etai;
	
	etai = eta_secondf(trace_ctx);
	etai_etat = bxdf->eta / etai;
	entering = vec_dot(wiw, hit.hit.geometric_normal) < 0.0;
	if (entering)
		etai_etat = etai / bxdf->eta;
	costheta = rt_min(vec_dot(vec_neg(wiw), hit.rel_shading_normal), 1.0);
	sintheta = rt_sqrt(1.0 - costheta*costheta);
	if (reflectance(costheta, etai_etat) > rt_random_float(&ctx->seed) || etai_etat * sintheta > 1.0)
	{
		*wow = reflect(wiw, hit.rel_shading_normal);
		*wow = clip(*wow, hit.rel_geometric_normal);
		/**wow = vec_sub(wiw, vec_scale(hit.relative_normal, 2.0 * vec_dot(wiw, hit.relative_normal)));*/
		return (filter_sample(world, bxdf->base.tex, hit.hit.uv));
	}
	else
	{
		*wow = vec_norm(refract(wiw, hit.rel_shading_normal, etai_etat));
		*wow = clip(*wow, vec_neg(hit.rel_geometric_normal));
		if (entering)
			eta_push(trace_ctx, prim_mat(hit.prim), bxdf->eta, (const GLOBAL t_bxdf_any *) bxdf - world->bxdfs);
		else
			eta_del(trace_ctx, prim_mat(hit.prim));
		return (filter_sample(world, bxdf->refraction_tex, hit.hit.uv));
	}
}

static t_vec
	f_bxdf_sample(const GLOBAL t_world *world, GLOBAL t_context *ctx, t_trace_ctx *trace_ctx, const GLOBAL t_bxdf *bxdf, t_world_hit hit, t_vec wiw, t_vec *wow)
{
	if (bxdf->type == RT_BXDF_DIFFUSE)
		return (f_bxdf_diffuse_sample(world, ctx, (const GLOBAL t_bxdf_diffuse *) bxdf, hit, wiw, wow));
	if (bxdf->type == RT_BXDF_REFLECTIVE)
		return (f_bxdf_reflective_sample(world, ctx, (const GLOBAL t_bxdf_reflective *) bxdf, hit, wiw, wow));
	if (bxdf->type == RT_BXDF_TRANSMISSIVE)
		return (f_bxdf_transmissive_sample(world, ctx, trace_ctx, (const GLOBAL t_bxdf_transmissive *) bxdf, hit, wiw, wow));;
	if (bxdf->type == RT_BXDF_BLINN_PHONG)
		return (f_bxdf_bphong_sample(world, ctx, (const GLOBAL t_bxdf_bphong *) bxdf, hit, wiw, wow));
	return vec_0();
}

t_vec
	f_bsdf_sample(const GLOBAL t_world *world, GLOBAL t_context *ctx, t_trace_ctx *trace_ctx, t_bsdf bsdf, t_world_hit hit, t_vec wiw, t_vec *wow)
{
	uint32_t			idx;
	float				rand;
	const GLOBAL t_bxdf	*bxdf;
	uint32_t			type;

	idx = bsdf.begin;
	rand = rt_random_float_range(&ctx->seed, 0.0, bsdf.weight);
	type = 0;
	if (vec_dot(wiw, hit.hit.geometric_normal) > 0 && eta_has(trace_ctx, prim_mat(hit.prim)))
	{
		return (vec_scale(f_bxdf_sample(world, ctx, trace_ctx,
						get_bxdf_const(world, trace_ctx->etas[eta_get(trace_ctx, prim_mat(hit.prim))].bxdf), hit, wiw, wow), bsdf.weight));
	}
	while (type < RT_BXDF_COUNT)
	{
		while (idx < bsdf.end)
		{
			bxdf = get_bxdf_const(world, idx);
			if (bxdf->type != type)
				break ;
			rand -= bxdf->weight;
			if (rand <= 0)
				return (vec_scale(f_bxdf_sample(world, ctx, trace_ctx, bxdf, hit, wiw, wow), bsdf.weight));
			idx += 1;
		}
		type += 1;
	}
	return (vec(0, 0, 0, 0));
}
