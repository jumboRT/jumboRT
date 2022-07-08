#include "world.h"

#ifndef ACCEL_NODE_STACK_SIZE
# define ACCEL_NODE_STACK_SIZE 64
#endif

#ifndef ACCEL_USE_TREE
# define ACCEL_USE_TREE 1
#endif

struct s_stack_node {
	uint32_t	index;
	FLOAT		max;
};

static void
	world_intersect_primitives(const GLOBAL t_world *world, t_ray ray, t_world_hit *hit)
{
	uint64_t					index;
	const GLOBAL char			*primitives;
	const GLOBAL t_primitive	*primitive;
	t_world_hit					current;

	index = 0;
	primitives = (const GLOBAL char*) world->primitives;
	while (index < world->primitives_size)
	{
		primitive = (const GLOBAL t_primitive *) (primitives + index);
		if (prim_intersect(primitive, world, ray, 0.001, &current) && current.hit.t < hit->hit.t)
			*hit = current;
		index += world_primitive_size(prim_type(primitive));
	}
}

static void
	world_intersect_degenerates(const GLOBAL t_world *world, t_ray ray, t_world_hit *hit)
{
	uint32_t					index;
	const GLOBAL t_primitive	*primitive;
	t_world_hit					current;

	index = 0;
	while (index < world->accel_degenerates_count)
	{
		primitive = get_prim_const(world, world->accel_degenerates[index]);
		if (prim_intersect(primitive, world, ray, 0.001, &current) && current.hit.t < hit->hit.t)
			*hit = current;
		index += 1;
	}
}

static void
	world_intersect_tree(const GLOBAL t_world *world, t_ray ray, t_world_hit *hit)
{
	struct s_stack_node			stack[ACCEL_NODE_STACK_SIZE];
	uint32_t					istack;
	const GLOBAL t_accel_node	*node;
	const GLOBAL t_primitive	*prim;
	uint32_t					iprim;
	const GLOBAL uint32_t		*prims;
	FLOAT						org_t;
	FLOAT						dir_t;
	FLOAT						split_t;
	FLOAT						min_t;
	FLOAT						max_t;
	FLOAT						plane_t;
	t_world_hit					current;

	min_t = 0.001;
	max_t = RT_HUGE_VAL;
	istack = 0;
	node = world->accel_nodes;
	while (min_t < hit->hit.t)
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
						stack[istack].index = node - world->accel_nodes + 1;
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
			prim = get_prim_const(world, prims[iprim]);
			if (prim_intersect(prim, world, ray, min_t, &current) && current.hit.t < hit->hit.t)
				*hit = current;
			iprim += 1;
		}
		if (istack == 0)
			return ;
		min_t = max_t;
		istack -= 1;
		node = world->accel_nodes + stack[istack].index;
		max_t = stack[istack].max;
	}
}

int
	world_intersect(const GLOBAL t_world *world, t_ray ray, t_world_hit *hit)
{
	hit->hit.t = RT_HUGE_VAL;
	if (ACCEL_USE_TREE)
	{
		world_intersect_degenerates(world, ray, hit);
		world_intersect_tree(world, ray, hit);
	}
	else
		world_intersect_primitives(world, ray, hit);
	return (hit->hit.t < RT_HUGE_VAL);
}
