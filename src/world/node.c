#include "world.h"

#if ACCEL_USE_ROPES

const GLOBAL uint32_t
	*node_prims(const GLOBAL t_world *world, const GLOBAL t_accel_node *node)
{
	if (nprims(*node) == 1)
		return (&world->leaf_data[node->a.leaf_data_index].a.one_primitive);
	else
		return (world->accel_indices + world->leaf_data[node->a.leaf_data_index].a.primitive_ioffset);
}

#else

const GLOBAL uint32_t
	*node_prims(const GLOBAL t_world *world, const GLOBAL t_accel_node *node)
{
	if (nprims(*node) == 1)
		return (&node->a.one_primitive);
	else
		return (world->accel_indices + node->a.primitive_ioffset);
}

#endif

float
	split_pos(t_accel_node node)
{
	return (node.a.split);
}

uint32_t
	nprims(t_accel_node node)
{
	return (node.b.nprims >> 2);
}

uint32_t
	split_axis(t_accel_node node)
{
	return (node.b.flags & 3);
}

uint32_t
	is_leaf(t_accel_node node)
{
	return (split_axis(node) == 3);
}

uint32_t
	above_child(t_accel_node node)
{
	return (node.b.above_child >> 2);
}

