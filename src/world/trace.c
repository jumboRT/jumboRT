#include "world.h"

t_vec
    ray_scatter(const GLOBAL t_material *mat, GLOBAL t_context *ctx, t_ray ray_in, t_world_hit hit)
{
	t_vec	reflected;
	FLOAT	cos_theta;
	FLOAT	sin_theta;
	FLOAT	r0;
	FLOAT	reflectance;
	FLOAT	rr;
	t_vec	out_perp;
	t_vec	out_parallel;

	if (mat->refractive)
	{
		rr = mat->refractive_index;
		if (vec_dot(ray_in.dir, hit.hit.normal) < 0)
		    rr = 1.0 / rr;
		cos_theta = rt_min(vec_dot(vec_neg(ray_in.dir), hit.relative_normal), 1.0);
		sin_theta = rt_sqrt(1.0 - cos_theta * cos_theta);
		r0 = (1.0 - rr) / (1 + rr);
		r0 = r0 * r0;
		reflectance = r0 + (1.0 - r0) * rt_pow(1.0 - cos_theta, 5);
		if (sin_theta * rr <= 1.0 && reflectance <= rt_random_float(&ctx->seed))
		{
			out_perp = vec_scale(vec_add(ray_in.dir, vec_scale(hit.relative_normal, cos_theta)), rr);
			out_parallel = vec_scale(hit.relative_normal, -rt_sqrt(rt_abs(1.0 - vec_mag2(out_perp))));
			return (vec_norm(vec_add(out_perp, out_parallel)));
		}
	}
	if (mat->reflective || mat->refractive) {
		reflected = vec_sub(ray_in.dir, vec_scale(hit.relative_normal, 2.0 * vec_dot(ray_in.dir, hit.relative_normal)));
		/* TODO: fuzzy is unset for refractive materials, but it's used here anyways */
		return (vec_norm(vec_add(reflected, rt_random_in_sphere(&ctx->seed, 0.0, mat->fuzzy))));
	}
	return (rt_random_on_hemi(&ctx->seed, hit.relative_normal));
}

t_vec get_albedo(const GLOBAL t_world *world, const GLOBAL t_material *mat, t_vec2 uv) {
	if (mat->has_texture) {
		return tex_sample(world, get_tex(world, mat->tex_offset), uv);
	}
	return mat->albedo;
}

t_vec
	world_trace(const GLOBAL t_world *world, GLOBAL t_context *ctx, t_ray ray, int depth)
{
	t_vec					head;
	t_vec					tail;
	t_world_hit				hit;
	const GLOBAL t_material	*mat;

	head = vec(1, 1, 1);
	tail = vec(0, 0, 0);
	while (depth > 0 && world_intersect(world, ray, &hit))
	{
		mat = get_mat_const(world, prim_mat(hit.prim));
		tail = vec_add(tail, vec_mul(head, mat->emission));
		head = vec_mul(head, get_albedo(world, mat, hit.hit.uv));
		ray.org = hit.hit.pos;
		ray.dir = ray_scatter(mat, ctx, ray, hit);
		depth -= 1;
	}
	return (tail);
}
