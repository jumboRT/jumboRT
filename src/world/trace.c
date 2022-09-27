#include "world.h"
#include "hit.h"
#include "shape.h"
#include "mat.h"

static void
	toggle_volume(const GLOBAL t_material **volumes, uint32_t *volume_size, const GLOBAL t_material *mat, float norm_dir)
{
	uint32_t	volume_index;

	if (norm_dir < 0 && *volume_size < RT_MAX_VOLUMES)
	{
		volumes[*volume_size] = mat;
		*volume_size += 1;
	}
	else if (norm_dir > 0)
	{
		volume_index = 0;
		while (volume_index < *volume_size)
		{
			if (volumes[volume_index] == mat)
			{
				*volume_size -= 1;
				volumes[volume_index] = volumes[*volume_size];
				break ;
			}
			volume_index += 1;
		}
	}
}

static void
	intersect_volume(const GLOBAL t_material *const *volumes, uint32_t volume_size, GLOBAL t_context *ctx, t_world_hit *hit)
{
	uint32_t	volume_index;
	float		t;

	volume_index = 0;
	while (volume_index < volume_size)
	{
		t = rt_log(1 - rt_random_float(&ctx->seed)) / -volumes[volume_index]->density;
		if (t < hit->hit.t)
		{
			hit->hit.t = t;
			hit->mat = volumes[volume_index];
			hit->is_volume = 1;
		}
		volume_index += 1;
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
	hit->rel_geometric_normal = hit->hit.geometric_normal;
	if (vec_dot(hit->hit.geometric_normal, ray.dir) > 0)
		hit->rel_geometric_normal = vec_neg(hit->rel_geometric_normal);
	hit->rel_shading_normal = hit->hit.shading_normal;
	if (hit->mat->flags & RT_MAT_HAS_NORMAL)
		hit->rel_shading_normal = local_to_world(*hit, vec_sub(vec_mul(sample_vector(world, hit->mat->normal_map, hit->hit.uv), vec(2, 2, 1, 1)), vec(1, 1, 0, 0)));
	if (hit->mat->flags & RT_MAT_HAS_BUMP)
		hit->rel_shading_normal = local_to_world(*hit, bump(world, hit->mat->bump_map, hit->hit.uv));
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
	hit->prim = 0;
	hit->mat = 0;
	hit->is_volume = 0;
	hit->hit.t = ctx->time;
	world_intersect(ctx->world, ray, hit);
	intersect_volume(ctx->volumes, ctx->volume_size, ctx->ctx, hit);
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
	else if (hit->hit.t >= ctx->time)
	{
		hit->hit.pos = ray_at(ray, hit->hit.t);
		if (ctx->world->flags & RT_WORLD_HAS_AMBIENT)
			hit->mat = get_mat_const(ctx->world, ctx->world->ambient_mat);
		hit->hit.uv = sphere_uv_at(ray.dir);
		init_normals(hit, ray, ray.dir);
	}
	else
	{
		hit->mat = get_mat_const(ctx->world, prim_mat(hit->prim));
		prim_hit_info(hit->prim, ctx->world, ray, hit);
		fix_normals(ctx->world, hit, ray);
	}
}

static int
	intersect_light(const t_trace_ctx *ctx, t_world_hit *hit, t_world_hit *world_hit)
{
	uint32_t	light;
	t_world_hit	light_hit;
	t_ray		light_ray;

	light = rt_random(&ctx->ctx->seed) % ctx->world->lights_count;
	hit->prim = get_prim_const(ctx->world, ctx->world->lights[light]);
	hit->mat = get_mat_const(ctx->world, prim_mat(hit->prim));
	hit->hit.pos = prim_sample(hit->prim, ctx->world, ctx->ctx);
	hit->hit.t = vec_mag(vec_sub(hit->hit.pos, world_hit->hit.pos));
	light_ray = ray(world_hit->hit.pos, vec_norm(vec_sub(hit->hit.pos, world_hit->hit.pos)));
	intersect_partial(ctx, &light_hit, light_ray);
	if (prim_type(hit->prim) == RT_SHAPE_POINT)
	{
		if (hit->hit.t > ctx->time || light_hit.hit.t < hit->hit.t)
			return (0);
		init_normals(hit, light_ray, light_ray.dir);
		hit->hit.uv = vec2(0, 0);
		return (1);
	}
	if (light_hit.hit.t >= ctx->time || hit->is_volume || vec_mag(vec_sub(hit->hit.pos, light_hit.hit.pos)) > RT_TINY_VAL)
		return (0);
	hit->mat = get_mat_const(ctx->world, prim_mat(hit->prim));
	prim_hit_info(hit->prim, ctx->world, light_ray, hit);
	fix_normals(ctx->world, hit, light_ray);
	return (1);
}

static int
	world_trace_debug(t_trace_ctx *ctx, const t_world_hit *hit)
{
	t_vec2 uv;

	if (ctx->world->render_mode == RT_RENDER_MODE_UV)
	{
		uv = vec2(rt_mod(rt_mod(u(hit->hit.uv), 1.0) + 1.0, 1.0), rt_mod(rt_mod(v(hit->hit.uv), 1.0) + 1.0, 1.0));
		ctx->tail = vec(u(uv), v(uv), 0, 0);
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

static t_vec
	get_emission(t_trace_ctx *ctx, const t_world_hit *hit)
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
	should_add_emission(t_trace_ctx *ctx, const t_world_hit *hit)
{
	(void) ctx;
	// TODO: don't add emission after a specular hit (or something similar)
	return (!hit->is_volume);
}

static int
	world_trace_step(t_trace_ctx *ctx)
{
	t_world_hit	hit;
	t_world_hit	light_hit;
	t_vec		dir;
	t_vec		scale;

	intersect_full(ctx, &hit, ctx->ray);
	if (world_trace_debug(ctx, &hit) || hit.mat == 0)
		return (0);
	ctx->ray.org = hit.hit.pos;
	if (should_add_emission(ctx, &hit))
		ctx->tail = vec_add(ctx->tail, vec_mul(ctx->head, get_emission(ctx, &hit)));
	if (intersect_light(ctx, &light_hit, &hit))
	{
		dir = vec_norm(vec_sub(hit.hit.pos, light_hit.hit.pos));
		scale = f_bsdf_f(ctx, &hit, ctx->ray.dir, dir);
		scale = vec_scale(scale, rt_abs(vec_dot(dir, light_hit.hit.geometric_normal)));
		scale = vec_scale(scale, prim_area(light_hit.prim, ctx->world));
		ctx->tail = vec_add(ctx->tail, vec_mul(ctx->head, vec_mul(get_emission(ctx, &hit), scale)));
	}
	if (!(hit.mat->flags & RT_MAT_HAS_ALPHA) || rt_random_float(&ctx->ctx->seed) < w(filter_sample(ctx->world, hit.mat->alpha, hit.hit.uv)))
		ctx->head = vec_mul(ctx->head, f_bsdf_sample(ctx, &hit, ctx->ray.dir, &ctx->ray.dir));
	if (!hit.is_volume && (hit.mat->flags & RT_MAT_HAS_VOLUME) && vec_dot(ctx->ray.dir, hit.rel_geometric_normal) < 0)
		toggle_volume(ctx->volumes, &ctx->volume_size, hit.mat, vec_dot(ctx->ray.dir, hit.hit.geometric_normal));
	return (!vec_eq(ctx->head, vec_0()));
}

void
	world_trace_init(const GLOBAL t_world *world, GLOBAL t_context *ctx, t_trace_ctx *tctx)
{
	const GLOBAL t_material	*ambient;

	tctx->world = world;
	tctx->ctx = ctx;
	tctx->head = vec(1, 1, 1, 1);
	tctx->tail = vec(0, 0, 0, 0);
	tctx->volume_size = 0;
	tctx->time = world->ambient_dist;
	if (world->flags & RT_WORLD_HAS_AMBIENT)
	{
		ambient = get_mat_const(world, world->ambient_mat);
		if ((ambient->flags & RT_MAT_HAS_VOLUME) && tctx->volume_size < RT_MAX_VOLUMES)
		{
			tctx->volumes[tctx->volume_size] = ambient;
			tctx->volume_size += 1;
		}
	}
	eta_init(tctx, 1.0);
}

t_vec
	world_trace(const GLOBAL t_world *world, GLOBAL t_context *ctx, t_ray ray, int depth)
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

