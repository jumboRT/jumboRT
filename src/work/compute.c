#include "work_compute.h"

#ifdef RT_OPENCL

__kernel void
	work_kernel(
		GLOBAL t_world *world,
		GLOBAL t_context *ctx,
		uint64_t begin,
		uint64_t end,
		GLOBAL t_result *results,
		GLOBAL void *primitives,
		GLOBAL void *materials,
		GLOBAL void *vertices,
		GLOBAL void *accel_nodes,
		GLOBAL void *accel_indices,
		GLOBAL void *accel_degenerates,
		GLOBAL void *texture_data,
		GLOBAL void *textures,
		GLOBAL void *bxdfs)
{
	uint64_t			index;
	uint64_t			size;
	GLOBAL t_context	*my_ctx;

	if (get_global_id(0) == 0)
	{
		world->primitives = primitives;
		world->materials = materials;
		world->vertices = vertices;
		world->accel_nodes = accel_nodes;
		world->accel_indices = accel_indices;
		world->accel_degenerates = accel_degenerates;
		world->texture_data = texture_data;
		world->textures = textures;
		world->bxdfs = bxdfs;
	}
	barrier(CLK_GLOBAL_MEM_FENCE);
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
	result.color = world_trace(world, ctx, ray, RT_MAX_DEPTH);
	result.index = index % (world->img_meta.width * world->img_meta.height);
	return (result);
}

