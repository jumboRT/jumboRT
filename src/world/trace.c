#include "world.h"

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

static const GLOBAL t_material
	*intersect_volume(const GLOBAL t_material **volumes, uint32_t volume_size, GLOBAL t_context *ctx, float *max_t)
{
	uint32_t				volume_index;
	const GLOBAL t_material	*mat;
	float					t;
	float					t2;

	volume_index = 0;
	mat = 0;
	t2 = *max_t;
	while (volume_index < volume_size)
	{
		t = rt_log(1 - rt_random_float(&ctx->seed)) / -volumes[volume_index]->density;
		if (t < t2)
		{
			*max_t = t;
			mat = volumes[volume_index];
		}
		volume_index += 1;
	}
	return (mat);
}

t_vec
	bump(const GLOBAL t_world *world, uint32_t bump_map, t_vec2 uv)
{
	float	dfdx;
	float	dfdy;

	dfdx = sample_float_offset(world, bump_map, uv, vec2(10, 0)); /* TODO check lower offset values */
	dfdy = sample_float_offset(world, bump_map, uv, vec2(0, 10));
	dfdx -= sample_float_offset(world, bump_map, uv, vec2(0, 0));
	dfdy -= sample_float_offset(world, bump_map, uv, vec2(0, 0));
	/*
	return (vec_scale(vec3(
				(-dfdx) / (rt_sqrt(dfdx * dfdx + dfdy * dfdy + 1)),
				(-dfdy) / (rt_sqrt(dfdx * dfdx + dfdy * dfdy + 1)),
				1 / (rt_sqrt(dfdx * dfdx + dfdy * dfdy + 1))), 1.0));
	*/
	return (vec_scale(vec(
				(-dfdx) / (rt_sqrt(dfdx * dfdx + dfdy * dfdy + 1)),
				(-dfdy) / (rt_sqrt(dfdx * dfdx + dfdy * dfdy + 1)),
				1 / (rt_sqrt(dfdx * dfdx + dfdy * dfdy + 1)),
				0.0),
				1.0));
}

/* TODO: unify these branches */
/* TODO: when hitting a material that only has emission, should the ray stop? */
/* TODO: maybe ambient lighting should be a material so you can start in a volume */
static int
	world_trace_step(const GLOBAL t_world *world, GLOBAL t_context *ctx, t_trace_ctx *tctx)
{
	t_world_hit				hit;
	const GLOBAL t_material	*mat;
	t_vec					bsdf;
	t_vec2					uv;

	if (!world_intersect(world, tctx->ray, &hit))
	{
		if (world->flags & RT_WORLD_HAS_AMBIENT)
		{
			mat = get_mat_const(world, world->ambient_mat);
			if (mat->flags & RT_MAT_EMITTER)
			{
				hit.hit.uv = sphere_uv_at(tctx->ray.dir);
				tctx->tail = vec_add(tctx->tail, vec_mul(tctx->head, vec_scale(filter_sample(world, mat->emission, hit.hit.uv), mat->brightness)));
			}
		}
		return (0);
	}
	if (world->render_mode == RT_RENDER_MODE_GEOMETRIC_NORMAL)
	{
		tctx->tail = vec_abs(hit.hit.geometric_normal);
		return (0);
	}
	if (world->render_mode == RT_RENDER_MODE_UV)
	{
		uv = vec2(rt_mod(rt_mod(u(hit.hit.uv), 1.0) + 1.0, 1.0), rt_mod(rt_mod(v(hit.hit.uv), 1.0) + 1.0, 1.0));
		tctx->tail = vec(u(uv), v(uv), 0, 0);
		return (0);
	}
	mat = intersect_volume(tctx->volumes, tctx->volume_size, ctx, &hit.hit.t);
	bsdf = vec(1, 1, 1, 1);
	if (mat != 0)
	{
		tctx->ray.org = ray_at(tctx->ray, hit.hit.t);
		hit.hit.geometric_normal = rt_random_on_hemi(&ctx->seed, tctx->ray.dir);
		hit.hit.shading_normal = hit.hit.geometric_normal;
		hit.hit.pos = tctx->ray.org;
		hit.rel_geometric_normal = hit.hit.geometric_normal;
		hit.rel_shading_normal = hit.hit.shading_normal;
		hit.hit.dpdu = vec_0();
		hit.hit.dpdv = vec_0();
		bsdf = f_bsdf_sample(world, ctx, tctx, mat->volume, hit, tctx->ray.dir, &tctx->ray.dir);
	}
	else
	{
		tctx->ray.org = hit.hit.pos;
		mat = get_mat_const(world, prim_mat(hit.prim));
		hit.rel_geometric_normal = hit.hit.geometric_normal;
		if (vec_dot(hit.hit.geometric_normal, tctx->ray.dir) > 0)
			hit.rel_geometric_normal = vec_neg(hit.rel_geometric_normal);
		hit.rel_shading_normal = hit.hit.shading_normal;
		if (mat->flags & RT_MAT_HAS_NORMAL)
			hit.rel_shading_normal = local_to_world(hit, vec_sub(vec_mul(sample_vector(world, mat->normal_map, hit.hit.uv), vec(2, 2, 1, 1)), vec(1, 1, 0, 0)));
		if (mat->flags & RT_MAT_HAS_BUMP)
			hit.rel_shading_normal = local_to_world(hit, bump(world, mat->bump_map, hit.hit.uv));
		hit.hit.shading_normal = hit.rel_shading_normal;
		if (world->render_mode == RT_RENDER_MODE_SHADING_NORMAL)
		{
			tctx->tail = vec_abs(hit.hit.shading_normal);
			return (0);
		}
		if ((mat->flags & RT_MAT_EMITTER) && mat->emission_exp != 0.0)
			tctx->tail = vec_add(tctx->tail, vec_mul(tctx->head, vec_scale(filter_sample(world, mat->emission, hit.hit.uv), mat->brightness * rt_pow(rt_abs(vec_dot(tctx->ray.dir, hit.rel_shading_normal)), mat->emission_exp))));
		else if (mat->flags & RT_MAT_EMITTER)
			tctx->tail = vec_add(tctx->tail, vec_mul(tctx->head, vec_scale(filter_sample(world, mat->emission, hit.hit.uv), mat->brightness)));
		if (((~mat->flags & RT_MAT_HAS_ALPHA) || rt_random_float(&ctx->seed) < w(filter_sample(world, mat->alpha, hit.hit.uv))) && mat->surface.end > mat->surface.begin)
		{
			if (vec_dot(hit.hit.geometric_normal, tctx->ray.dir) > 0)
				hit.rel_shading_normal = vec_neg(hit.rel_shading_normal);
			bsdf = f_bsdf_sample(world, ctx, tctx, mat->surface, hit, tctx->ray.dir, &tctx->ray.dir);
		}
		if ((mat->flags & RT_MAT_HAS_VOLUME) && vec_dot(tctx->ray.dir, hit.rel_geometric_normal) < 0)
			toggle_volume(tctx->volumes, &tctx->volume_size, mat, vec_dot(tctx->ray.dir, hit.hit.geometric_normal));
	}
	tctx->head = vec_mul(tctx->head, bsdf);
	return (!vec_eq(tctx->head, vec_0()));
}

void
	world_trace_init(const GLOBAL t_world *world, t_trace_ctx *tctx)
{
	const GLOBAL t_material	*ambient;

	tctx->head = vec(1, 1, 1, 1);
	tctx->tail = vec(0, 0, 0, 0);
	tctx->volume_size = 0;
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

	world_trace_init(world, &tctx);
	tctx.ray = ray;
	while (depth > 0 && world_trace_step(world, ctx, &tctx))
		depth -= 1;
	return (tctx.tail);
}

void
	world_trace_all(const GLOBAL t_world *world, GLOBAL t_context *ctx, GLOBAL t_result *results, uint64_t index, uint64_t begin, uint64_t end, uint64_t stride)
{
	int			depth;
	t_trace_ctx	tctx;

	depth = 0;
	while (index < end - begin)
	{
		if (depth == 0)
		{
			depth = RT_MAX_DEPTH;
			world_trace_init(world, &tctx);
			tctx.ray = project(world, ctx, begin + index);
		}
		depth -= 1;
		if (!world_trace_step(world, ctx, &tctx))
			depth = 0;
		if (depth == 0)
		{
			results[index].color = tctx.tail;
			index += stride;
		}
	}
}

