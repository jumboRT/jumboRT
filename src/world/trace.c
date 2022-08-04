#include "world.h"

t_vec
	bump(const GLOBAL t_world *world, uint32_t bump_map, t_vec2 uv)
{
	FLOAT	dfdx;
	FLOAT	dfdy;

	dfdx = tex_samplef_id_offset(world, bump_map, uv, vec2(2, 0));
	dfdy = tex_samplef_id_offset(world, bump_map, uv, vec2(0, 2));
	dfdx -= tex_samplef_id_offset(world, bump_map, uv, vec2(0, 0));
	dfdy -= tex_samplef_id_offset(world, bump_map, uv, vec2(0, 0));
	return (vec_scale(vec3(
				(-dfdx) / (rt_sqrt(dfdx * dfdx + dfdy * dfdy + 1)),
				(-dfdy) / (rt_sqrt(dfdx * dfdx + dfdy * dfdy + 1)),
				1 / (rt_sqrt(dfdx * dfdx + dfdy * dfdy + 1))), 1.0));
}

t_vec
	world_trace(const GLOBAL t_world *world, GLOBAL t_context *ctx, t_ray ray, int depth)
{
	t_vec					head;
	t_vec					tail;
	t_world_hit				hit;
	t_vec					new_dir;
	t_vec					bsdf;
	const GLOBAL t_material	*mat;

	head = vec(1, 1, 1, 1);
	tail = vec(0, 0, 0, 0);
	while (depth > 0 && world_intersect(world, ray, &hit))
	{
		ray.org = hit.hit.pos;
		mat = get_mat_const(world, prim_mat(hit.prim));
		if ((~mat->flags & RT_MAT_HAS_ALPHA) || rt_random_float(&ctx->seed) < w(tex_sample_id(world, mat->alpha_tex, hit.hit.uv)))
		{
			if (mat->flags & RT_MAT_HAS_NORMAL)
				hit.hit.normal = vec_norm(tex_sample_id(world, mat->normal_map, hit.hit.uv));
			if (mat->flags & RT_MAT_HAS_BUMP)
				hit.hit.normal = vec_norm(vec_add(hit.hit.normal, bump(world, mat->bump_map, hit.hit.uv)));
			hit.relative_normal = hit.hit.normal;
			if (vec_dot(hit.geometric_normal, hit.relative_normal) < 0)
				hit.relative_normal = vec_neg(hit.hit.normal);
				/*hit.relative_normal = vec_norm(vec_add(hit.relative_normal, bump(world, mat->bump_map, hit.hit.uv)));*/
			if (mat->flags & RT_MAT_EMITTER)
				tail = vec_add(tail, vec_mul(head, vec_scale(tex_sample_id(world, mat->emission, hit.hit.uv), mat->brightness)));
			bsdf = f_bsdf_sample(world, ctx, *mat, hit, ray.dir, head, &new_dir);
			head = vec_mul(head, bsdf);
			ray.dir = new_dir;
		}
		depth -= 1;
	}
	return (tail);
}
