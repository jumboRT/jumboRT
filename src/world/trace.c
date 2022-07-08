#include "world.h"

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
		ray.dir = rt_random_on_hemi(&ctx->seed, hit.relative_normal);
		depth -= 1;
	}
	return (tail);
}
