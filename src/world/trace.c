#include "world.h"

t_vec
    ray_scatter(const GLOBAL t_material *mat, GLOBAL t_context *ctx, t_ray ray_in, t_world_hit hit)
{
	t_vec	reflected;
	FLOAT	cos_theta;
	FLOAT	rr;
	t_vec	out_perp;
	t_vec	out_parallel;

	if (mat->refractive_index != 1.0)
	{
	    rr = mat->refractive_index;
	    if (vec_dot(hit.relative_normal, hit.hit.normal) > 0)
		rr = 1.0 / rr;
	    cos_theta = rt_min(vec_dot(ray_in.dir, hit.relative_normal), 1.0);
	    out_perp = vec_scale(vec_add(ray_in.dir, vec_scale(hit.relative_normal, cos_theta)), rr);
	}
	else if (mat->reflective) {
	    reflected = vec_sub(ray_in.dir, vec_scale(normal, 2.0 * vec_dot(ray_in.dir, normal)));
	    return (vec_norm(reflected + rt_random_in_sphere(&ctx->seed, 0.0, mat->fuzzy)));
	}
	return (rt_random_on_hemi(&ctx->seed, normal));
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
		head = vec_mul(head, mat->albedo);
		ray.org = hit.hit.pos;
		ray.dir = ray_scatter(mat, ctx, ray, hit.relative_normal);
		depth -= 1;
	}
	return (tail);
}
