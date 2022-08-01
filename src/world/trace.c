#include "world.h"

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
		mat = get_mat_const(world, prim_mat(hit.prim));
		if (mat->flags & RT_MAT_EMITTER)
			tail = vec_add(tail, vec_mul(head, vec_scale(tex_sample_id(world, mat->emission, hit.hit.uv), mat->brightness)));
		if (!f_bsdf_sample(world, ctx, *mat, hit, ray.dir, head, &new_dir, &bsdf))
			break ;
		/*bsdf = f_bsdf(world, *mat, hit, ray.dir, new_dir);*/
		head = vec_mul(head, bsdf);
		ray.org = hit.hit.pos;
		ray.dir = new_dir;
		depth -= 1;
	}
	return (tail);
}
