#include "world.h"

#ifndef ACCEL_NODE_STACK_SIZE
# define ACCEL_NODE_STACK_SIZE 1024
#endif

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
	world_intersect_triangle(const t_world *world, const void *ptr, t_ray ray, t_hit *hit)
{
	const t_shape_triangle	*tr;

	tr = (const t_shape_triangle *) ptr;
	return (ray_triangle_intersect(
				ray,
				triangle(
					world->vertices[tr->a].pos,
					world->vertices[tr->b].pos,
					world->vertices[tr->c].pos),
				RT_RAY_MIN,
				hit));
}

static void
	world_intersect_primitive(const t_world *world, const t_primitive *primitive, t_ray ray, t_hit *hit)
{
	t_hit	current_hit;

	current_hit.t = RT_HUGE_VAL;
	if (primitive->shape_type == RT_SHAPE_SPHERE)
		world_intersect_sphere(primitive, ray, &current_hit);
	else
		world_intersect_triangle(world, primitive, ray, &current_hit);
	if (current_hit.t < hit->t)
		*hit = current_hit;
}

/*
static int
	world_intersect_primitives(const t_world *world, t_ray ray, t_hit *hit)
{
	uint64_t				index;
	const char				*primitives;
	const t_primitive		*primitive;

	index = 0;
	primitives = (const char*) world->primitives;
	hit->t = RT_HUGE_VAL;
	while (index < world->primitives_size)
	{
		primitive = (const t_primitive *) (primitives + index);
		world_primitive_intersect(world, primitive, ray, hit);
		index += world_primitive_size(primitive);
	}
	return (hit->t < RT_HUGE_VAL);
}
*/

static int
	world_intersect_primitives(const t_world *world, t_ray ray, t_hit *hit)
{
	uint32_t	node_stack[ACCEL_NODE_STACK_SIZE];
	uint32_t	node_stack_index;
	uint32_t	node;

	hit->t = RT_HUGE_VAL;
	node = 0;
	while (1)
	{
		while (!is_leaf(world->accel_nodes[node]))
		{
			
		}
	}
	return (0);
}

int
	world_intersect(const t_world *world, t_ray ray, t_hit *hit) {
	return (world_intersect_primitives(world, ray, hit));
}
