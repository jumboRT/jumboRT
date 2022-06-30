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
}

__kernel void
	work_kernel(GLOBAL t_world *world, GLOBAL t_context *ctx, uint64_t begin, uint64_t end, GLOBAL t_result *results)
{
	uint64_t	index;
	uint64_t	size;

	size = end - begin;
	index = get_global_id(0);
	while (index < size)
	{
		results[index] = work_compute(world, ctx, begin + index);
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
	FLOAT		t;
	t_vec		top;
	t_vec		bot;
	t_hit		hit;

	ray = project(world, ctx, index);
	if (world_intersect(world, ray, &hit))
	{
		result.color = vec_scale(vec_add(hit.normal, vec(1, 1, 1)), 0.5);
	}
	else
	{
		t = z(ray.dir) * 0.5 + 0.5;
		top = vec(0.5, 0.7, 1.0);
		bot = vec(1.0, 1.0, 1.0);
		result.color = vec_add(vec_scale(bot, 1 - t), vec_scale(top, t));
	}
	result.index = index % (world->img_meta.width * world->img_meta.height);
	return (result);
}
