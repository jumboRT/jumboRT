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

	dfdx = sample_float_offset(world, bump_map, uv, vec2(2, 0)) * 5;
	dfdy = sample_float_offset(world, bump_map, uv, vec2(0, 2)) * 5;
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

static void
	fix_normals(const GLOBAL t_world *world, const GLOBAL t_material *mat, t_world_hit *hit, t_ray ray)
{
	hit->rel_geometric_normal = hit->hit.geometric_normal;
	if (vec_dot(hit->hit.geometric_normal, ray.dir) > 0)
		hit->rel_geometric_normal = vec_neg(hit->rel_geometric_normal);
	hit->rel_shading_normal = hit->hit.shading_normal;
	if (mat->flags & RT_MAT_HAS_NORMAL)
		hit->rel_shading_normal = local_to_world(*hit, vec_sub(vec_mul(sample_vector(world, mat->normal_map, hit->hit.uv), vec(2, 2, 1, 1)), vec(1, 1, 0, 0)));
	if (mat->flags & RT_MAT_HAS_BUMP)
		hit->rel_shading_normal = local_to_world(*hit, bump(world, mat->bump_map, hit->hit.uv));
	hit->hit.shading_normal = hit->rel_shading_normal;
}

/* TODO: alpha */
static int
	ray_to_light(const GLOBAL t_world *world, const GLOBAL t_primitive *prim, GLOBAL t_context *ctx, t_light_hit *hit, t_vec org)
{
	t_vec					pos;
	t_world_hit				world_hit;
	t_ray					ray_obj;
	const GLOBAL t_material	*mat;
	float					scale;
	float					dot;

	mat = get_mat_const(world, prim_mat(prim));
	if (prim_is_infinite(prim) || !(mat->flags & RT_MAT_EMITTER))
		return (0);
	pos = prim_sample(prim, world, ctx);
	ray_obj = ray(org, vec_norm(vec_sub(pos, org)));
	if (prim_type(prim) == RT_SHAPE_POINT)
	{
		if (world_intersect(world, ray_obj, &world_hit) && vec_mag(vec_sub(pos, world_hit.hit.pos)) < RT_TINY_VAL)
			return (0);
		hit->color = vec_scale(filter_sample(world, mat->emission, vec2(0, 0)), mat->brightness * RT_PI);
		return (1);
	}
	else
	{
		if (!world_intersect(world, ray_obj, &world_hit) || vec_mag(vec_sub(pos, world_hit.hit.pos)) < RT_TINY_VAL)
			return (0);
		fix_normals(world, mat, &world_hit, ray_obj);
		dot = rt_abs(vec_dot(ray_obj.dir, world_hit.rel_shading_normal));
		scale = mat->brightness * dot * prim_area(prim, world);
		if (mat->emission_exp != 0.0)
			scale *= rt_pow(dot, mat->emission_exp);
		hit->color = vec_scale(filter_sample(world, mat->emission, world_hit.hit.uv), scale);
		return (1);
	}
}

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
	t_vec		result;
	uint64_t	i;
	int			this_depth;

	result = vec_0();
	i = 0;
	while (i < world->trace_batch_size)
	{
		this_depth = depth;
		world_trace_init(world, &tctx);
		tctx.ray = ray;
		while (this_depth > 0 && world_trace_step(world, ctx, &tctx))
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
			world_trace_init(world, &tctx);
			tctx.ray = project(world, ctx, begin + my_index);
		}
		depth -= 1;
		if (!world_trace_step(world, ctx, &tctx))
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

