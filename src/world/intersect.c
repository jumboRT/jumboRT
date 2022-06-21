#include "world.h"

#include <stdio.h>

static int
	world_intersect_sphere(const void *ptr, t_ray ray, t_hit *hit)
{
	const t_shape_sphere	*sp;

	sp = (const t_shape_sphere *) ptr;
	return (ray_sphere_intersect(
				ray,
				sphere(
					sp->pos,
					sp->radius),
				RT_RAY_MIN,
				hit));
}

static int
	world_intersect_triangle(const void *ptr, t_ray ray, t_hit *hit)
{
	/*
	const t_shape_triangle	*tr;

	tr = (const t_shape_triangle *) ptr;
	return (ray_triangle_intersect(
				ray,
				triangle(
					vec_0(),
					vec_0(),
					vec_0()),
				RT_RAY_MIN,
				hit));
	*/
	(void) ptr;
	(void) ray;
	(void) hit;
	return (0);
}

static int
	world_intersect_primitives(const t_world *world, t_ray ray, t_hit *hit)
{
	uint64_t				index;
	const char				*primitives;
	const t_primitive		*primitive;
	t_hit					current_hit;

	index = 0;
	primitives = (const char*) world->primitives;
	hit->t = RT_HUGE_VAL;
	while (index < world->primitives_size) {
		primitive = (const t_primitive *) (primitives + index);
		if (primitive->shape_type == RT_SHAPE_SPHERE)
		{
			if (world_intersect_sphere(primitive, ray, &current_hit) && current_hit.t < hit->t)
			{
				*hit = current_hit;
			}
			index += (sizeof(t_shape_sphere) + RT_PRIMITIVE_ALIGN - 1) / RT_PRIMITIVE_ALIGN * RT_PRIMITIVE_ALIGN;
		}
		else
		{
			if (world_intersect_triangle(primitive, ray, &current_hit) && current_hit.t < hit->t)
			{
				*hit = current_hit;
			}
			index += (sizeof(t_shape_triangle) + RT_PRIMITIVE_ALIGN - 1) / RT_PRIMITIVE_ALIGN * RT_PRIMITIVE_ALIGN;
		}
	}
	return (hit->t < RT_HUGE_VAL);
}

int
	world_intersect(const t_world *world, t_ray ray, t_hit *hit) {
	return (world_intersect_primitives(world, ray, hit));
}
