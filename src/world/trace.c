#include "world.h"
#include "hit.h"
#include "shape.h"
#include "mat.h"
#include "bsdf.h"

#ifndef RT_TRACE_LIGHT_SAMPLING
# define RT_TRACE_LIGHT_SAMPLING 1
#endif

#ifndef RT_TRACE_LIGHT_SLOW
# define RT_TRACE_LIGHT_SLOW 8
#endif

static void
	toggle_volume(t_trace_ctx *ctx, const t_world_hit *hit, t_vec new_dir)
{
	uint32_t	vi;
	float		id;
	float		od;

	if (hit->is_volume || !(hit->mat->flags & RT_MAT_HAS_VOLUME))
		return ;
	id = vec_dot(hit->hit.geometric_normal, ctx->ray.dir);
	od = vec_dot(hit->hit.geometric_normal, new_dir);
	if (id < 0 && od < 0 && ctx->volume_size < RT_MAX_VOLUMES)
	{
		ctx->volumes[ctx->volume_size] = hit->mat;
		ctx->volume_size += 1;
	}
	else if (id > 0 && od > 0)
	{
		vi = 0;
		while (vi < ctx->volume_size)
		{
			if (ctx->volumes[vi] == hit->mat)
			{
				ctx->volume_size -= 1;
				ctx->volumes[vi] = ctx->volumes[ctx->volume_size];
				break ;
			}
			vi += 1;
		}
	}
}

static void
	intersect_volume(const t_trace_ctx *ctx, t_world_hit *hit)
{
	uint32_t	vi;
	float		t;

	vi = 0;
	while (vi < ctx->volume_size)
	{
		t = rt_log(rt_random_float(&ctx->ctx->seed))
			/ -ctx->volumes[vi]->density;
		if (t < hit->hit.t)
		{
			hit->hit.t = t;
			hit->prim = 0;
			hit->mat = ctx->volumes[vi];
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

	(void) ray;
	if (hit->mat->flags & RT_MAT_HAS_NORMAL)
	{
		tmp = sample_vector(world, hit->mat->normal_map, hit->hit.uv);
		tmp = vec_sub(vec_mul(tmp, vec(2, 2, 1, 1)), vec(1, 1, 0, 0));
		hit->hit.shading_normal = local_to_world(hit, tmp);
	}
	if (hit->mat->flags & RT_MAT_HAS_BUMP)
	{
		tmp = bump(world, hit->mat->bump_map, hit->hit.uv);
		hit->hit.shading_normal = local_to_world(hit, tmp);
	}
}

static void
	init_normals(t_world_hit *hit, t_ray ray, t_vec normal)
{
	(void) ray;
	hit->hit.geometric_normal = normal;
	hit->hit.shading_normal = normal;
	hit->hit.dpdu = vec_tangent(hit->hit.geometric_normal);
	hit->hit.dpdv = vec_cross(hit->hit.geometric_normal, hit->hit.dpdu);
}

static int
	alpha_test(const t_trace_ctx *ctx, t_world_hit *hit)
{
	float	alpha;
	float	sample;

	if (hit->mat == 0 || hit->is_volume)
		return (1);
	if (!(hit->mat->flags & RT_MAT_HAS_ALPHA))
		return (1);
	sample = rt_random_float(&ctx->ctx->seed);
	alpha = w(filter_sample(ctx->world, hit->mat->alpha, hit->hit.uv));
	return (sample < alpha);
}

static void
	intersect_partial(const t_trace_ctx *ctx, t_world_hit *hit,
			t_ray ray, float max)
{
	hit->hit.t = max;
	hit->prim = 0;
	world_intersect(ctx->world, ray, hit);
	hit->mat = 0;
	hit->is_volume = 0;
	hit->is_ambient = 0;
	intersect_volume(ctx, hit);
	if (hit->hit.t >= max)
		hit->is_ambient = 1;
}

static void
	intersect_info(const t_trace_ctx *ctx, t_world_hit *hit,
			t_ray ray)
{
	if (hit->is_volume)
	{
		hit->hit.uv = vec2(0, 0);
		hit->hit.pos = ray_at(ray, hit->hit.t);
		init_normals(hit, ray, rt_random_on_hemi(&ctx->ctx->seed, vec_neg(ray.dir)));
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

static void
	intersect_full(const t_trace_ctx *ctx, t_world_hit *hit,
			t_ray ray, float max)
{
	intersect_partial(ctx, hit, ray, max);
	intersect_info(ctx, hit, ray);
}

static void
	intersect_slow(const t_trace_ctx *ctx, t_world_hit *hit,
			t_ray ray, float max)
{
	t_trace_ctx	ctx2;
	int			depth;
	float		t;

	ctx2 = *ctx;
	ctx2.ray = ray;
	depth = 0;
	t = 0;
	while (depth < RT_TRACE_LIGHT_SLOW)
	{
		intersect_partial(&ctx2, hit, ray, max - t);
		if (hit->is_volume)
			break ;
		if (hit->is_ambient)
		{
			t = max;
			break ;
		}
		prim_hit_info(hit->prim, ctx->world, ray, hit);
		fix_normals(ctx->world, hit, ray);
		t += hit->hit.t;
		if (alpha_test(&ctx2, hit))
			break ;
		toggle_volume(&ctx2, hit, ray.dir);
		ray.org = hit->hit.pos;
		depth += 1;
	}
	hit->hit.t = t;
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

	if (ctx->world->lights_count == 0)
		return (0);
	lray = intersect_light_init(ctx, hit, whit);
	if (!RT_TRACE_LIGHT_SLOW)
		intersect_partial(ctx, &lhit, lray, ctx->time);
	else
		intersect_slow(ctx, &lhit, lray, ctx->time);
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
	if (!RT_TRACE_LIGHT_SLOW)
	{
		prim_hit_info(hit->prim, ctx->world, lray, &lhit);
		fix_normals(ctx->world, &lhit, lray);
	}
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
	brightness = hit->mat->brightness * rt_pow(rt_abs(vec_dot(ctx->ray.dir, hit->hit.shading_normal)), hit->mat->emission_exp);
	return (vec_scale(sample, brightness));
}

static int
	world_trace_debug(t_trace_ctx *ctx, const t_world_hit *hit)
{
	float		uv_u;
	float		uv_v;
	t_sample	sample;
	t_bxdf_ctx	bxdf_ctx;

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
		if (vec_dot(hit->hit.geometric_normal, ctx->ray.dir) > 0)
			ctx->tail = vec_scale(ctx->tail, 0.25);
		return (1);
	}
	if (ctx->world->render_mode == RT_RENDER_MODE_SHADING_NORMAL)
	{
		ctx->tail = vec_abs(hit->hit.shading_normal);
		if (vec_dot(hit->hit.shading_normal, ctx->ray.dir) > 0)
			ctx->tail = vec_scale(ctx->tail, 0.25);
		return (1);
	}
	if (ctx->world->render_mode == RT_RENDER_MODE_ALBEDO)
	{
		if (hit->mat == 0)
		{
			ctx->tail = vec_0();
			return (1);
		}
		bxdf_ctx.ctx = ctx;
		bxdf_ctx.hit = hit;
		bxdf_ctx.wi = ctx->ray.dir;
		sample = bsdf_sample(&bxdf_ctx);
		ctx->tail = vec_abs(vec_scale(sample.bsdf, RT_PI));
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
	t_bxdf_ctx	bxdf_ctx;

	if (!intersect_light(ctx, &lhit, hit))
		return ;
	sample.wo = vec_norm(vec_sub(lhit.hit.pos, hit->hit.pos));
	bxdf_ctx.ctx = ctx;
	bxdf_ctx.hit = hit;
	bxdf_ctx.wi = ctx->ray.dir;
	sample.bsdf = bsdf_f(&bxdf_ctx, sample.wo);
	sample.pdf = rt_abs(vec_dot(sample.wo, lhit.hit.shading_normal));
	sample.pdf *= rt_abs(vec_dot(sample.wo, hit->hit.shading_normal));
	sample.pdf *= prim_area(lhit.prim, ctx->world);
	sample.pdf *= ctx->world->lights_count;
	sample.pdf /= rt_max(lhit.hit.t * lhit.hit.t, RT_TINY_VAL);
	tmp = vec_scale(vec_mul(le(ctx, &lhit), sample.bsdf), sample.pdf);
	ctx->tail = vec_add(ctx->tail, vec_mul(ctx->head, tmp));
}

static int
	world_trace_step(t_trace_ctx *ctx)
{
	t_world_hit	hit;
	t_sample	sample;
	t_bxdf_ctx	bxdf_ctx;

	intersect_full(ctx, &hit, ctx->ray, ctx->time);
	if (world_trace_debug(ctx, &hit) || hit.mat == 0)
		return (0);
	sample.wo = ctx->ray.dir;
	if (alpha_test(ctx, &hit))
	{
		ctx->alpha_skips = 0;
		if (RT_TRACE_LIGHT_SAMPLING)
			world_trace_light(ctx, &hit);
		if (!hit.is_volume && (hit.is_ambient || ctx->specref || prim_is_degenerate(hit.prim)))
			ctx->tail = vec_add(ctx->tail, vec_mul(ctx->head, le(ctx, &hit)));
		bxdf_ctx.ctx = ctx;
		bxdf_ctx.hit = &hit;
		bxdf_ctx.wi = ctx->ray.dir;
		sample = bsdf_sample(&bxdf_ctx);
		if (sample.pdf == 0)
			return (0);
		ctx->head = vec_mul(ctx->head, vec_scale(sample.bsdf, rt_abs(vec_dot(sample.wo, hit.hit.shading_normal)) / sample.pdf));
		if (RT_TRACE_LIGHT_SAMPLING)
			ctx->specref = bxdf_is_perfspec(sample.bxdf);
		else
			ctx->specref = 1;
	}
	else
	{
		ctx->alpha_skips += 1;
		if (ctx->alpha_skips >= RT_TRACE_LIGHT_SLOW)
			ctx->specref = 1;
	}
	toggle_volume(ctx, &hit, sample.wo);
	ctx->ray.org = hit.hit.pos;
	ctx->ray.dir = sample.wo;
	ctx->time -= hit.hit.t;
	return (!vec_eq(ctx->head, vec_0()) && ctx->time > 0);
}

void
	world_trace_init(const GLOBAL t_world *world,
			GLOBAL t_context *ctx, t_trace_ctx *tctx)
{
	const GLOBAL t_material	*ambient;
	int						max_volumes;

	max_volumes = RT_MAX_VOLUMES;
	tctx->world = world;
	tctx->ctx = ctx;
	tctx->head = vec(1, 1, 1, 1);
	tctx->tail = vec(0, 0, 0, 0);
	tctx->volume_size = 0;
	tctx->time = world->ambient_dist;
	tctx->specref = 1;
	tctx->alpha_skips = 0;
	if (world->flags & RT_WORLD_HAS_AMBIENT)
	{
		ambient = get_mat_const(world, world->ambient_mat);
		if ((ambient->flags & RT_MAT_HAS_VOLUME) && max_volumes > 0)
		{
			tctx->volumes[tctx->volume_size] = ambient;
			tctx->volume_size += 1;
		}
	}
}

t_vec
	world_trace(const GLOBAL t_world *world, GLOBAL t_context *ctx,
			uint64_t begin, int depth)
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
		tctx.ray = project(world, ctx, begin + i);
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

