#include "world.h"

#ifndef ACCEL_NODE_STACK_SIZE
# define ACCEL_NODE_STACK_SIZE 64
#endif

struct s_stack_node {
	uint32_t	index;
	float		max;
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
		if (prim_intersect(primitive, world, ray, RT_TINY_VAL, &current) && current.hit.t < hit->hit.t)
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
		if (prim_intersect(primitive, world, ray, RT_TINY_VAL, &current) && current.hit.t < hit->hit.t)
			*hit = current;
		index += 1;
	}
}

#if ACCEL_USE_ROPES

static void
	world_intersect_tree(const GLOBAL t_world *world, t_ray ray, t_world_hit *hit)
{
	const GLOBAL t_accel_node	*node;
	const GLOBAL t_primitive	*prim;
	uint32_t					iprim;
	const GLOBAL uint32_t		*prims;
	uint32_t					nprim;
	float						min_t;
	float						org_t;
	float						dir_t;
	float						split_t;
	t_world_hit					current;
	uint32_t					exit_rope;
	float						exit_distance;
	uint32_t					index;
	float						distance;
	uint32_t					rope_index;

	min_t = RT_TINY_VAL;
	node = world->accel_nodes;
	iprim = 0;
	nprim = 0;
	while (min_t < hit->hit.t)
	{
		if (iprim >= nprim)
		{
			while (!is_leaf(*node))
			{
				org_t = xyz(ray.org, split_axis(*node));
				dir_t = xyz(ray.dir, split_axis(*node));
				split_t = split_pos(*node);
				if (org_t + dir_t * min_t < split_t)
					node = node + 1;
				else
					node = world->accel_nodes + above_child(*node);
			}
			if (nprims(*node) == 1)
				prims = &node->a.one_primitive;
			else
				prims = world->accel_indices + node->a.primitive_ioffset;
			iprim = 0;
			nprim = nprims(*node);
		}
		if (iprim < nprim)
		{
			prim = get_prim_const(world, prims[iprim]);
			if (prim_intersect(prim, world, ray, min_t, &current) && current.hit.t < hit->hit.t)
				*hit = current;
			iprim += 1;
		}
		if (iprim >= nprim)
		{
			exit_rope = 0xFFFFFFFF;
			exit_distance = RT_HUGE_VAL;
			index = 0;
			while (index < 3)
			{
				org_t = xyz(ray.org, index);
				dir_t = xyz(ray.dir, index);
				if (dir_t < 0)
					rope_index = index + 0;
				else if (dir_t > 0)
					rope_index = index + 3;
				else
				{
					index += 1;
					continue ;
				}
				distance = (node->rope_data.bounds[rope_index] - org_t) / dir_t;
				if (distance < exit_distance)
				{
					exit_distance = distance;
					exit_rope = node->rope_data.ropes[rope_index];
				}
				index += 1;
			}
			if (exit_rope == 0xFFFFFFFF)
				return ;
			node = world->accel_nodes + exit_rope;
			min_t = rt_max(min_t, exit_distance - RT_TINY_VAL);
		}
	}
}

#else

static void
	world_intersect_tree(const GLOBAL t_world *world, t_ray ray, t_world_hit *hit)
{
	struct s_stack_node			stack[ACCEL_NODE_STACK_SIZE];
	uint32_t					istack;
	const GLOBAL t_accel_node	*node;
	const GLOBAL t_primitive	*prim;
	uint32_t					iprim;
	const GLOBAL uint32_t		*prims;
	uint32_t					nprim;
	float						org_t;
	float						dir_t;
	float						split_t;
	float						min_t;
	float						max_t;
	float						plane_t;
	float						tmp;
	uint32_t					next_child;
	t_world_hit					current;

	min_t = RT_TINY_VAL;
	max_t = RT_HUGE_VAL;
	istack = 0;
	node = world->accel_nodes;
	iprim = 0;
	nprim = 0;
	while (min_t < hit->hit.t)
	{
		if (iprim >= nprim)
		{
			while (!is_leaf(*node))
			{
				org_t = xyz(ray.org, split_axis(*node));
				dir_t = xyz(ray.dir, split_axis(*node));
				split_t = split_pos(*node);
				if (org_t + dir_t * min_t < split_t)
				{
					next_child = above_child(*node);
					node = node + 1;
					tmp = dir_t;
				}
				else
				{
					next_child = node - world->accel_nodes + 1;
					node = world->accel_nodes + above_child(*node);
					tmp = -dir_t;
				}
				plane_t = (split_t - org_t) / dir_t;
				if (tmp > 0 && plane_t < max_t)
				{
					stack[istack].index = next_child;
					stack[istack].max = max_t;
					istack += 1;
					max_t = plane_t;
				}
			}
			if (nprims(*node) == 1)
				prims = &node->a.one_primitive;
			else
				prims = world->accel_indices + node->a.primitive_ioffset;
			iprim = 0;
			nprim = nprims(*node);
		}
		if (iprim < nprim)
		{
			prim = get_prim_const(world, prims[iprim]);
			if (prim_intersect(prim, world, ray, min_t, &current) && current.hit.t < hit->hit.t)
				*hit = current;
			iprim += 1;
		}
		if (iprim >= nprim)
		{
			if (istack == 0)
				return ;
			min_t = rt_max(min_t, max_t - RT_TINY_VAL);
			istack -= 1;
			node = world->accel_nodes + stack[istack].index;
			max_t = stack[istack].max;
		}
	}
}

#endif

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
	if (hit->hit.t < RT_HUGE_VAL)
	{
		prim_hit_info(hit->prim, world, ray, hit);
		return (1);
	}
	else
		return (0);
}
