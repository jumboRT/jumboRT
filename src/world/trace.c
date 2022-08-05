#include "world.h"

#ifndef RT_MAX_VOLUMES
# define RT_MAX_VOLUMES 1
#endif

static void
	toggle_volume(const GLOBAL t_material **volumes, uint32_t *volume_size, const GLOBAL t_material *mat, FLOAT norm_dir)
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
	*intersect_volume(const GLOBAL t_material **volumes, uint32_t volume_size, GLOBAL t_context *ctx, FLOAT *max_t)
{
	uint32_t				volume_index;
	const GLOBAL t_material	*mat;
	FLOAT					t;
	FLOAT					t2;

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
	FLOAT	dfdx;
	FLOAT	dfdy;

	dfdx = tex_samplef_id_offset(world, bump_map, uv, vec2(10, 0));
	dfdy = tex_samplef_id_offset(world, bump_map, uv, vec2(0, 10));
	dfdx -= tex_samplef_id_offset(world, bump_map, uv, vec2(0, 0));
	dfdy -= tex_samplef_id_offset(world, bump_map, uv, vec2(0, 0));
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

t_vec
	world_trace(const GLOBAL t_world *world, GLOBAL t_context *ctx, t_ray ray, int depth)
{
	t_vec					head;
	t_vec					tail;
	t_world_hit				hit;
	t_vec					new_dir;
	const GLOBAL t_material	*mat;
	const GLOBAL t_material *volumes[RT_MAX_VOLUMES];
	uint32_t				volume_size;
	t_vec					bsdf;

	head = vec(1, 1, 1, 1);
	tail = vec(0, 0, 0, 0);
	volume_size = 0;
	while (depth > 0 && world_intersect(world, ray, &hit))
	{
		mat = intersect_volume(volumes, volume_size, ctx, &hit.hit.t);
		if (mat != 0)
		{
			ray.org = ray_at(ray, hit.hit.t);
			hit.hit.normal = rt_random_on_hemi(&ctx->seed, ray.dir);
			hit.relative_normal = hit.hit.normal;
			head = vec_mul(head, f_bsdf_sample(world, ctx, mat->volume_bxdf_begin, mat->volume_bxdf_end, hit, ray.dir, head, &new_dir));
			ray.dir = new_dir;
		}
		else
		{
			ray.org = hit.hit.pos;
			mat = get_mat_const(world, prim_mat(hit.prim));
			if (mat->flags & RT_MAT_EMITTER)
				tail = vec_add(tail, vec_mul(head, vec_scale(tex_sample_id(world, mat->emission, hit.hit.uv), mat->brightness)));
			if (((~mat->flags & RT_MAT_HAS_ALPHA) || rt_random_float(&ctx->seed) < w(tex_sample_id(world, mat->alpha_tex, hit.hit.uv))) && mat->bxdf_end > mat->bxdf_begin)
			{
				if (mat->flags & RT_MAT_HAS_NORMAL)
				{
					hit.relative_normal = local_to_world(hit, vec_norm(tex_sample_id(world, mat->normal_map, hit.hit.uv)));
					/*hit.relative_normal = (vec_norm(tex_sample_id(world, mat->normal_map, hit.hit.uv)));
					hit.relative_normal = vec_neg(local_to_world(hit, hit.relative_normal));*/
				}
				if (mat->flags & RT_MAT_HAS_BUMP)
				{
					/*
					hit.relative_normal = vec_norm(vec_add(hit.relative_normal, local_to_world(hit, bump(world, mat->bump_map, hit.hit.uv))));
					*/
					hit.relative_normal = vec_norm(local_to_world(hit, bump(world, mat->bump_map, hit.hit.uv)));
				}
				/*
				return vec(rt_abs(x(hit.relative_normal)), rt_abs(y(hit.relative_normal)), rt_abs(z(hit.relative_normal)), 0);
				*/
				if (vec_dot(hit.geometric_normal, hit.relative_normal) < 0)
					hit.relative_normal = vec_neg(hit.relative_normal);
				bsdf = f_bsdf_sample(world, ctx, mat->bxdf_begin, mat->bxdf_end, hit, ray.dir, head, &new_dir);
				if (vec_eq(bsdf, vec_0()))
					break;
				head = vec_mul(head, bsdf);
				ray.dir = new_dir;
			}
			else if (mat->flags & RT_MAT_HAS_VOLUME)
				toggle_volume(volumes, &volume_size, mat, vec_dot(ray.dir, hit.hit.normal));
		}
		depth -= 1;
	}
	return (tail);
}
