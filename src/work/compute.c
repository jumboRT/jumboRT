#include "work_compute.h"

#ifdef RT_OPENCL

__kernel void
	work_kernel(
		GLOBAL t_world *world,
		GLOBAL t_context *ctx,
		uint64_t begin,
		uint64_t end,
		GLOBAL t_result *results,
		GLOBAL unsigned int *index,
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
	GLOBAL t_context	*my_ctx;

	world->primitives = primitives;
	world->materials = materials;
	world->vertices = vertices;
	world->accel_nodes = accel_nodes;
	world->accel_indices = accel_indices;
	world->accel_degenerates = accel_degenerates;
	world->texture_data = texture_data;
	world->textures = textures;
	world->bxdfs = bxdfs;
	my_ctx = &ctx[get_global_id(0)];
	world_trace_all(world, my_ctx, results, index, begin, end);
}

#endif

uint64_t
	project_index(const GLOBAL t_world *world, uint64_t index)
{
	return (index / world->batch_size % (world->img_meta.width * world->img_meta.height));
}

t_ray
	project(const GLOBAL t_world *world, GLOBAL t_context *ctx, uint64_t index)
{
	const GLOBAL t_camera		*cam;
	const GLOBAL t_image_meta	*meta;
	t_vec						u;
	t_vec						v;
	t_vec						blur;
	t_ray						ray;

	cam = &world->camera;
	meta = &world->img_meta;
	index = project_index(world, index);
	u = vec_scale(cam->u, rt_random_float(&ctx->seed) + (uint64_t) (index % meta->width));
	v = vec_scale(cam->v, rt_random_float(&ctx->seed) + (uint64_t) (index / meta->width));
	blur = rt_random_in_disk(&ctx->seed, cam->u_norm, cam->v_norm, cam->blur);
	ray.org = vec_add(cam->org, blur);
	ray.dir = vec_norm(vec_sub(vec_add(cam->base, vec_add(u, v)), blur));
	return (ray);
}

t_result
	work_compute(GLOBAL t_world *world, GLOBAL t_context *ctx, uint64_t index)
{
	t_result	result;
	t_ray		ray;

	ray = project(world, ctx, index);
	result.color = world_trace(world, ctx, ray, RT_MAX_DEPTH);
	return (result);
}

