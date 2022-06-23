#include "world.h"

void
	leaf_create(t_accel_node *leaf, uint32_t *prim_indices, uint32_t nprim, uint32_t *indices)
{
	leaf->b.flags = 3;
	leaf->b.nprims = nprim;
}

void
	interior_create(t_accel_node *interior, uint32_t axis, uint32_t above_child, FLOAT offset)
{
	interior->a.split = offset;
	interior->b.flags = axis;
	interior->b.above_child |= (above_child << 2);
}

void
	world_accel(t_world *world)
{
	uint32_t	max_depth;

	
}
