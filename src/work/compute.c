#include "work_compute.h"

uint64_t
	project_index(const GLOBAL t_world *world, uint64_t index)
{
	return (index / world->batch_size
		% (world->img_meta.width * world->img_meta.height));
}

t_ray
	project(const GLOBAL t_world *world, GLOBAL t_context *ctx, uint64_t index)
{
	const GLOBAL t_camera		*cam;
	const GLOBAL t_image_meta	*meta;
	t_vec						uv[2];
	t_vec						blur;
	t_ray						ray;

	cam = &world->camera;
	meta = &world->img_meta;
	index = project_index(world, index);
	uv[0] = vec_scale(cam->u, rt_random_float(&ctx->seed)
			+ (uint64_t)(index % meta->width));
	uv[1] = vec_scale(cam->v, rt_random_float(&ctx->seed)
			+ (uint64_t)(index / meta->width));
	blur = rt_random_in_disk(&ctx->seed, cam->u_norm, cam->v_norm, cam->blur);
	ray.org = vec_add(cam->org, blur);
	ray.dir = vec_norm(vec_sub(vec_add(cam->base,
					vec_add(uv[0], uv[1])), blur));
	return (ray);
}

t_result
	work_compute(GLOBAL t_world *world, GLOBAL t_context *ctx, uint64_t index)
{
	t_result	result;

	result.color = world_trace(world, ctx, index, RT_MAX_DEPTH);
	return (result);
}
