#include "world.h"

const GLOBAL uint32_t
	*node_prims(const GLOBAL t_world *world, const GLOBAL t_accel_node *node)
{
	if (nprims(*node) <= ACCEL_INLINE_PRIMS)
		return (node->a.inline_primitives);
	else
		return (world->accel_indices + node->a.primitive_offset);
}

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

