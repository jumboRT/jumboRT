#include "accel.h"

#include "world.h"

const GLOBAL uint32_t
	*node_prims(const GLOBAL t_world *world, const GLOBAL t_accel_node *node)
{
	if (nprims(*node) <= ACCEL_INLINE_PRIMS)
		return (node->a.inline_primitives);
	else
		return (world->accel_indices + node->a.primitive_offset);
}
