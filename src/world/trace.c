#include "world.h"
#include "hit.h"
#include "shape.h"
#include "mat.h"
#include "bsdf.h"

static void
	toggle_volume(const GLOBAL t_material **volumes, uint32_t *volume_size,
			const GLOBAL t_material *mat, float norm_dir)
{
	uint32_t	vi;

	if (norm_dir < 0 && *volume_size < RT_MAX_VOLUMES)
	{
		volumes[*volume_size] = mat;
		*volume_size += 1;
	}
	else if (norm_dir > 0)
	{
		vi = 0;
		while (vi < *volume_size)
		{
			if (volumes[vi] == mat)
			{
				*volume_size -= 1;
				volumes[vi] = volumes[*volume_size];
				break ;
			}
			vi += 1;
		}
	}
}

static void
	intersect_volume(const GLOBAL t_material *const *volumes,
			uint32_t volume_size, GLOBAL t_context *ctx, t_world_hit *hit)
{
	uint32_t	vi;
	float		t;

	vi = 0;
	while (vi < volume_size)
	{
		t = rt_log(1 - rt_random_float(&ctx->seed)) / -volumes[vi]->density;
		if (t < hit->hit.t)
		{
			hit->hit.t = t;
			hit->mat = volumes[vi];
			hit->is_volume = 1;
		}
		vi += 1;
	}
}

t_vec
	bump(const GLOBAL t_world *world, uint32_t bump_map, t_vec2 uv)
{
	float	dfdx;
	float	dfdy;

	dfdx = sample_float_offset(world, bump_map, uv, vec2(2, 0)) * 5;
	dfdy = sample_float_offset(world, bump_map, uv, vec2(0, 2)) * 5;
	dfdx -= sample_float_offset(world, bump_map, uv, vec2(0, 0));
	dfdy -= sample_float_offset(world, bump_map, uv, vec2(0, 0));
	return (vec_scale(vec(
				(-dfdx) / (rt_sqrt(dfdx * dfdx + dfdy * dfdy + 1)),
				(-dfdy) / (rt_sqrt(dfdx * dfdx + dfdy * dfdy + 1)),
				1 / (rt_sqrt(dfdx * dfdx + dfdy * dfdy + 1)),
				0.0),
				1.0));
}

static void
	fix_normals(const GLOBAL t_world *world, t_world_hit *hit, t_ray ray)
{
	t_vec	tmp;

	hit->rel_geometric_normal = hit->hit.geometric_normal;
	if (vec_dot(hit->hit.geometric_normal, ray.dir) > 0)
		hit->rel_geometric_normal = vec_neg(hit->rel_geometric_normal);
	hit->rel_shading_normal = hit->hit.shading_normal;
	if (hit->mat->flags & RT_MAT_HAS_NORMAL)
	{
		tmp = sample_vector(world, hit->mat->normal_map, hit->hit.uv);
		tmp = vec_sub(vec_mul(tmp, vec(2, 2, 1, 1)), vec(1, 1, 0, 0));
		hit->rel_shading_normal = local_to_world(hit, tmp);
	}
	if (hit->mat->flags & RT_MAT_HAS_BUMP)
	{
		tmp = bump(world, hit->mat->bump_map, hit->hit.uv);
		hit->rel_shading_normal = local_to_world(hit, tmp);
	}
	hit->hit.shading_normal = hit->rel_shading_normal;
	if (vec_dot(hit->hit.shading_normal, ray.dir) > 0)
		hit->rel_shading_normal = vec_neg(hit->rel_shading_normal);
}

static void
	init_normals(t_world_hit *hit, t_ray ray, t_vec normal)
{
	hit->hit.geometric_normal = normal;
	hit->hit.shading_normal = normal;
	hit->rel_geometric_normal = normal;
	if (vec_dot(hit->hit.geometric_normal, ray.dir) > 0)
		hit->rel_geometric_normal = vec_neg(hit->rel_geometric_normal);
	hit->rel_shading_normal = normal;
	if (vec_dot(hit->hit.shading_normal, ray.dir) > 0)
		hit->rel_shading_normal = vec_neg(hit->rel_shading_normal);
	hit->hit.dpdu = vec_tangent(hit->hit.geometric_normal);
	hit->hit.dpdv = vec_cross(hit->hit.geometric_normal, hit->hit.dpdu);
}

static void
	intersect_partial(const t_trace_ctx *ctx, t_world_hit *hit, t_ray ray)
{
	hit->hit.t = ctx->time;
	hit->prim = 0;
	world_intersect(ctx->world, ray, hit);
	hit->mat = 0;
	hit->is_volume = 0;
	hit->is_ambient = 0;
	intersect_volume(ctx->volumes, ctx->volume_size, ctx->ctx, hit);
	if (hit->hit.t >= ctx->time)
		hit->is_ambient = 1;
}

static void
	intersect_full(const t_trace_ctx *ctx, t_world_hit *hit, t_ray ray)
{
	intersect_partial(ctx, hit, ray);
	if (hit->is_volume)
	{
		hit->hit.uv = vec2(0, 0);
		init_normals(hit, ray, rt_random_on_hemi(&ctx->ctx->seed, ray.dir));
	}
	else if (hit->is_ambient)
	{
		hit->hit.pos = ray_at(ray, hit->hit.t);
		if (ctx->world->flags & RT_WORLD_HAS_AMBIENT)
			hit->mat = get_mat_const(ctx->world, ctx->world->ambient_mat);
		hit->hit.uv = sphere_uv_at(ray.dir);
		init_normals(hit, ray, ray.dir);
	}
	else
	{
		prim_hit_info(hit->prim, ctx->world, ray, hit);
		fix_normals(ctx->world, hit, ray);
	}
}

static t_ray
	intersect_light_init(const t_trace_ctx *ctx, t_world_hit *hit,
			const t_world_hit *whit)
{
	uint32_t	light;
	t_vec		delta;

	light = rt_random(&ctx->ctx->seed) % ctx->world->lights_count;
	hit->prim = get_prim_const(ctx->world, ctx->world->lights[light]);
	hit->mat = get_mat_const(ctx->world, prim_mat(hit->prim));
	hit->hit.pos = prim_sample(hit->prim, ctx->world, ctx->ctx);
	delta = vec_sub(hit->hit.pos, whit->hit.pos);
	hit->hit.t = vec_mag(delta);
	return (ray(whit->hit.pos, vec_norm(delta)));
}

static int
	intersect_light(const t_trace_ctx *ctx, t_world_hit *hit,
			const t_world_hit *whit)
{
	t_world_hit	lhit;
	t_ray		lray;

	lray = intersect_light_init(ctx, hit, whit);
	intersect_partial(ctx, &lhit, lray);
	if (prim_type(hit->prim) == RT_SHAPE_POINT)
	{
		if (hit->hit.t > ctx->time || lhit.hit.t < hit->hit.t)
			return (0);
		hit->hit.uv = vec2(0, 0);
		init_normals(hit, lray, lray.dir);
		return (1);
	}
	if (lhit.hit.t >= ctx->time || lhit.is_volume || lhit.prim != hit->prim)
		return (0);
	if (vec_mag(vec_sub(hit->hit.pos, lhit.hit.pos)) > RT_TINY_VAL)
		return (0);
	prim_hit_info(hit->prim, ctx->world, lray, &lhit);
	fix_normals(ctx->world, &lhit, lray);
	*hit = lhit;
	return (1);
}

static t_vec
	le(t_trace_ctx *ctx, const t_world_hit *hit)
{
	t_vec	sample;
	float	brightness;

	if (!(hit->mat->flags & RT_MAT_EMITTER))
		return (vec_0());
	sample = filter_sample(ctx->world, hit->mat->emission, hit->hit.uv);
	brightness = hit->mat->brightness * rt_pow(rt_abs(vec_dot(ctx->ray.dir, hit->rel_shading_normal)), hit->mat->emission_exp);
	return (vec_scale(sample, brightness));
}

static int
	world_trace_debug(t_trace_ctx *ctx, const t_world_hit *hit)
{
	float	uv_u;
	float	uv_v;

	if (ctx->world->render_mode == RT_RENDER_MODE_UV)
	{
		uv_u = rt_mod(rt_mod(u(hit->hit.uv), 1.0f) + 1.0f, 1.0f);
		uv_v = rt_mod(rt_mod(v(hit->hit.uv), 1.0f) + 1.0f, 1.0f);
		ctx->tail = vec(uv_u, uv_v, 0, 0);
		return (1);
	}
	if (ctx->world->render_mode == RT_RENDER_MODE_GEOMETRIC_NORMAL)
	{
		ctx->tail = vec_abs(hit->hit.geometric_normal);
		return (1);
	}
	if (ctx->world->render_mode == RT_RENDER_MODE_SHADING_NORMAL)
	{
		ctx->tail = vec_abs(hit->hit.shading_normal);
		return (1);
	}
	return (0);
}

static void
	world_trace_light(t_trace_ctx *ctx, const t_world_hit *hit)
{
	t_world_hit	lhit;
	t_sample	sample;
	t_vec		tmp;

	if (!intersect_light(ctx, &lhit, hit))
		return ;
	sample.wo = vec_norm(vec_sub(hit->hit.pos, lhit.hit.pos));
	sample.bsdf = bsdf_f(ctx, hit, ctx->ray.dir, sample.wo);
	sample.pdf = bsdf_pdf(ctx, hit, ctx->ray.dir, sample.wo);
	sample.pdf *= rt_abs(vec_dot(sample.wo, lhit.hit.geometric_normal));
	sample.pdf *= prim_area(lhit.prim, ctx->world);
	sample.pdf /= lhit.hit.t * lhit.hit.t;
	tmp = vec_scale(vec_mul(le(ctx, &lhit), sample.bsdf), sample.pdf);
	ctx->tail = vec_add(ctx->tail, vec_mul(ctx->head, tmp));
}

static int
	world_trace_alpha(t_trace_ctx *ctx, t_world_hit *hit)
{
	float	alpha;
	float	sample;

	if (!(hit->mat->flags & RT_MAT_HAS_ALPHA))
		return (1);
	sample = rt_random_float(&ctx->ctx->seed);
	alpha = w(filter_sample(ctx->world, hit->mat->alpha, hit->hit.uv));
	return (sample < alpha);
}

#include <assert.h>
static int
	world_trace_step(t_trace_ctx *ctx)
{
	t_world_hit	hit;
	t_sample	sample;

	intersect_full(ctx, &hit, ctx->ray);
	if (world_trace_debug(ctx, &hit) || hit.mat == 0)
		return (0);
	world_trace_light(ctx, &hit);
	if (hit.is_ambient || (!hit.is_volume && ctx->should_add_emission))
		ctx->tail = vec_add(ctx->tail, vec_mul(ctx->head, le(ctx, &hit)));
	if (world_trace_alpha(ctx, &hit))
	{
		sample = bsdf_sample(ctx, &hit, ctx->ray.dir);
		if (sample.pdf == 0)
			return (0);
		ctx->ray.dir = sample.wo;
		// assert(vec_dot(sample.wo, hit.rel_shading_normal) >= 0);
		// TODO: scale by cosine
		ctx->head = vec_mul(ctx->head, vec_scale(sample.bsdf, rt_abs(vec_dot(sample.wo, hit.rel_shading_normal)) / sample.pdf));
	}
	if (!hit.is_volume && (hit.mat->flags & RT_MAT_HAS_VOLUME)
			&& vec_dot(ctx->ray.dir, hit.rel_geometric_normal) < 0)
		toggle_volume(ctx->volumes, &ctx->volume_size, hit.mat,
				vec_dot(ctx->ray.dir, hit.hit.geometric_normal));
	ctx->ray.org = hit.hit.pos;
	ctx->should_add_emission = bsdf_is_perfspec(&hit);
	return (!vec_eq(ctx->head, vec_0()));
}

void
	world_trace_init(const GLOBAL t_world *world,
			GLOBAL t_context *ctx, t_trace_ctx *tctx)
{
	const GLOBAL t_material	*ambient;

	tctx->world = world;
	tctx->ctx = ctx;
	tctx->head = vec(1, 1, 1, 1);
	tctx->tail = vec(0, 0, 0, 0);
	tctx->volume_size = 0;
	tctx->time = world->ambient_dist;
	tctx->should_add_emission = 1;
	tctx->refractive_index = 1.0f;
	if (world->flags & RT_WORLD_HAS_AMBIENT)
	{
		ambient = get_mat_const(world, world->ambient_mat);
		if ((ambient->flags & RT_MAT_HAS_VOLUME) && !!RT_MAX_VOLUMES)
		{
			tctx->volumes[tctx->volume_size] = ambient;
			tctx->volume_size += 1;
		}
	}
}

t_vec
	world_trace(const GLOBAL t_world *world, GLOBAL t_context *ctx,
			t_ray ray, int depth)
{
	t_trace_ctx	tctx;
	t_vec		result;
	uint64_t	i;
	int			this_depth;

	result = vec_0();
	i = 0;
	while (i < world->trace_batch_size)
	{
		this_depth = depth;
		world_trace_init(world, ctx, &tctx);
		tctx.ray = ray;
		while (this_depth > 0 && world_trace_step(&tctx))
			this_depth -= 1;
		result = vec_add(result, tctx.tail);
		i += 1;
	}
	return (vec_scale(result, 1.0 / world->trace_batch_size));
}

#ifdef RT_OPENCL

void
	world_trace_all(const GLOBAL t_world *world, GLOBAL t_context *ctx, GLOBAL t_result *results, GLOBAL unsigned int *index, uint64_t begin, uint64_t end)
{
	int				depth;
	t_trace_ctx		tctx;
	t_vec			result;
	unsigned int	my_index;
	uint64_t		i;

	depth = 0;
	i = world->trace_batch_size;
	my_index = atomic_add(index, 1);
	while (my_index < end - begin)
	{
		if (depth == 0)
		{
			if (i == world->trace_batch_size)
			{
				i = 0;
				result = vec_0();
			}
			depth = RT_MAX_DEPTH;
			world_trace_init(world, ctx, &tctx);
			tctx.ray = project(world, ctx, begin + my_index);
		}
		depth -= 1;
		if (!world_trace_step(&tctx))
			depth = 0;
		if (depth == 0)
		{
			i += 1;
			result = vec_add(result, tctx.tail);
			if (i == world->trace_batch_size)
			{
				results[my_index].color = vec_scale(result, 1.0 / world->trace_batch_size);
				my_index = atomic_add(index, 1);
			}
		}
	}
}

#endif

