#include "work_compute.h"

#ifdef RT_OPENCL

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

t_ray
	project(GLOBAL t_world *world, GLOBAL t_context *ctx, uint64_t index)
{
	GLOBAL t_image_meta	*meta;
	FLOAT				u;
	FLOAT				v;

	(void) ctx;
	meta = &world->img_meta;
	index = index % (meta->width * meta->height);
	u = (uint64_t) (index % meta->width) / (FLOAT) meta->width * 2 - 1;
	v = (uint64_t) (index / meta->width) / (FLOAT) meta->height * 2 - 1;
	u *= (FLOAT) meta->width / meta->height;
	return (ray(vec(0, 0, 0), vec_norm(vec(1, u, -v))));
}

t_result
	work_compute(GLOBAL t_world *world, GLOBAL t_context *ctx, uint64_t index)
{
	t_result	result;
	t_ray		ray;
	FLOAT		t;
	t_vec		top;
	t_vec		bot;
	t_sphere	sphere_obj;
	t_hit		hit;

	sphere_obj = sphere(vec(1, 0, 0), 0.5);
	ray = project(world, ctx, index);
	if (ray_sphere_intersect(ray, sphere_obj, 0.001, &hit))
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
