#include "accel.h"

#include "world.h"

float
	split_pos(t_accel_node node)
{
	return (node.u_a.split);
}

uint32_t
	nprims(t_accel_node node)
{
	return (node.u_b.nprims >> 2);
}

uint32_t
	split_axis(t_accel_node node)
{
	return (node.u_b.flags & 3);
}

uint32_t
	is_leaf(t_accel_node node)
{
	return (split_axis(node) == 3);
}

uint32_t
	above_child(t_accel_node node)
{
	return (node.u_b.above_child >> 2);
}
