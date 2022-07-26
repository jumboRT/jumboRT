#include "work_compute.h"

#ifdef RT_OPENCL

__kernel void
	set_ptr_kernel(GLOBAL t_world *world, int offset, GLOBAL void *ptr)
{
	if (offset == 0)
		world->primitives = ptr;
	if (offset == 1)
		world->materials = ptr;
	if (offset == 2)
		world->vertices = ptr;
	if (offset == 3)
		world->accel_nodes = ptr;
	if (offset == 4)
		world->accel_indices = ptr;
	if (offset == 5)
		world->accel_degenerates = ptr;
	if (offset == 6)
		world->texture_data = ptr;
	if (offset == 7)
		world->textures = ptr;
}

/* TODO: synchronize or duplicate context */
__kernel void
	work_kernel(GLOBAL t_world *world, GLOBAL t_context *ctx, uint64_t begin, uint64_t end, GLOBAL t_result *results)
{
	uint64_t			index;
	uint64_t			size;
	GLOBAL t_context	*my_ctx;

	size = end - begin;
	index = get_global_id(0);
	my_ctx = &ctx[index];
	while (index < size)
	{
		results[index] = work_compute(world, my_ctx, begin + index);
		index += get_global_size(0);
	}
}

#endif

static t_ray
	project(GLOBAL t_world *world, GLOBAL t_context *ctx, uint64_t index)
{
	GLOBAL t_camera		*cam;
	GLOBAL t_image_meta	*meta;
	t_vec				u;
	t_vec				v;

	(void) ctx;
	cam = &world->camera;
	meta = &world->img_meta;
	u = vec_scale(cam->u, rt_random_float(&ctx->seed) + index % meta->width);
	v = vec_scale(cam->v, rt_random_float(&ctx->seed) + index / meta->width % meta->height);
	return (ray(cam->org, vec_norm(vec_add(cam->base, vec_add(u, v)))));
}

t_result
	work_compute(GLOBAL t_world *world, GLOBAL t_context *ctx, uint64_t index)
{
	t_result	result;
	t_ray		ray;

	ray = project(world, ctx, index);
	result.color = world_trace(world, ctx, ray, 8); // TODO: RT_MAX_DEPTH
	result.index = index % (world->img_meta.width * world->img_meta.height);
	return (result);
}
