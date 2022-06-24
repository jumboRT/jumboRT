#include "world.h"

#ifndef ACCEL_NODE_STACK_SIZE
# define ACCEL_NODE_STACK_SIZE 64
#endif

struct s_stack_node {
	uint32_t	index;
	FLOAT		max;
};

static int
	world_intersect_sphere(const void *ptr, t_ray ray, FLOAT min, t_hit *hit)
{
	const t_shape_sphere	*sp;

	sp = (const t_shape_sphere *) ptr;
	return (ray_sphere_intersect(
				ray,
				sphere(
					sp->pos,
					sp->radius),
				min,
				hit));
}

static int
	world_intersect_triangle(const t_world *world, const void *ptr, t_ray ray, FLOAT min, t_hit *hit)
{
	const t_shape_triangle	*tr;

	tr = (const t_shape_triangle *) ptr;
	return (ray_triangle_intersect(
				ray,
				triangle(
					world->vertices[tr->a].pos,
					world->vertices[tr->b].pos,
					world->vertices[tr->c].pos),
				min,
				hit));
}

static void
	world_intersect_primitive(const t_world *world, const t_primitive *primitive, t_ray ray, FLOAT min, t_hit *hit)
{
	t_hit	current_hit;
	int		did_hit;

	current_hit.t = RT_HUGE_VAL;
	if (primitive->shape_type == RT_SHAPE_SPHERE)
		did_hit = world_intersect_sphere(primitive, ray, min, &current_hit);
	else
		did_hit = world_intersect_triangle(world, primitive, ray, min, &current_hit);
	if (did_hit && current_hit.t < hit->t)
		*hit = current_hit;
}

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
		world_intersect_primitive(world, primitive, ray, 0.001, hit);
		index += world_primitive_size(primitive->shape_type);
	}
	return (hit->t < RT_HUGE_VAL);
}

static int
	world_intersect_primitives2(const t_world *world, t_ray ray, t_hit *hit)
{
	struct s_stack_node	stack[ACCEL_NODE_STACK_SIZE];
	uint32_t			istack;
	const t_accel_node	*node;
	const t_primitive	*prim;
	uint32_t			iprim;
	const uint32_t		*prims;
	FLOAT				org_t;
	FLOAT				dir_t;
	FLOAT				split_t;
	FLOAT				min_t;
	FLOAT				max_t;
	FLOAT				plane_t;

	hit->t = RT_HUGE_VAL;
	min_t = 0.001;
	max_t = RT_HUGE_VAL;
	istack = 0;
	node = world->accel_nodes;
	while (min_t < hit->t)
	{
		while (!is_leaf(*node))
		{
			org_t = xyz(ray.org, split_axis(*node));
			dir_t = xyz(ray.dir, split_axis(*node));
			split_t = split_pos(*node);
			/* TODO: unify these branches */
			if (org_t + dir_t * min_t < split_t)
			{
				if (dir_t > 0)
				{
					plane_t = (split_t - org_t) / dir_t;
					if (plane_t < max_t)
					{
						stack[istack].index = above_child(*node);
						stack[istack].max = max_t;
						istack += 1;
						max_t = plane_t;
					}
				}
				node = node + 1;
			}
			else
			{
				if (dir_t < 0)
				{
					plane_t = (split_t - org_t) / dir_t;
					if (plane_t < max_t)
					{
						stack[istack].index = world->accel_nodes - node + 1;
						stack[istack].max = max_t;
						istack += 1;
						max_t = plane_t;
					}
				}
				node = world->accel_nodes + above_child(*node);
			}
		}
		if (nprims(*node) == 1)
			prims = &node->a.one_primitive;
		else
			prims = world->accel_indices + node->a.primitive_ioffset;
		iprim = 0;
		while (iprim < nprims(*node))
		{
			prim = (const t_primitive *) ((const char *) world->primitives + prims[iprim] * RT_PRIMITIVE_ALIGN);
			world_intersect_primitive(world, prim, ray, min_t, hit);
			iprim += 1;
		}
		if (istack == 0)
			return (hit->t < RT_HUGE_VAL);
		min_t = max_t;
		istack -= 1;
		node = world->accel_nodes + stack[istack].index;
		max_t = stack[istack].max;
	}
	return (1);
}

int
	world_intersect(const t_world *world, t_ray ray, t_hit *hit)
{
	return (world_intersect_primitives2(world, ray, hit));
}
