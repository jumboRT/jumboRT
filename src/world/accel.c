#include "world.h"

#include <math.h>
#include <stddef.h>

typedef struct s_accel_ctx {
	uint32_t	next;
	uint32_t	max;
	uint32_t	max_primitives;
	t_world		*world;
}	t_accel_ctx;

typedef struct s_node_ctx {
	uint64_t	offest;
	uint64_t	indices_count;
	t_bounds	bounds;
	uint32_t	*indices;
}	t_node_ctx;

void
	leaf_create(t_accel_node *leaf, const uint32_t *in_indices, uint32_t in_count, uint32_t *out_indices)
{
	(void) leaf;
	(void) in_indices;
	(void) in_count;
	(void) out_indices;
}

void
	interior_create(t_accel_node *interior, uint32_t axis, uint32_t above_child, FLOAT offset)
{
	interior->a.split = offset;
	interior->b.flags = axis;
	interior->b.above_child |= (above_child << 2);
}

void
	*vec_push_back(void *vec, uint64_t *vec_size, const void *val, uint64_t val_size)
{
	(void) vec;
	(void) vec_size;
	(void) val;
	(void) val_size;
	return NULL;
}

void
	build_tree(t_accel_ctx *accel_ctx, t_node_ctx node_ctx, uint32_t depth)
{
	t_accel_node	node;

	if (node_ctx.indices_count <= accel_ctx->max_primitives < depth == 0)
	{
		leaf_create(&node, node_ctx.indices, node_ctx.indices_count, accel_ctx->world->accel_indices);
		accel_ctx->world->accel_indices
				= vec_push_back(
						accel_ctx->world->accel_indices,
						&accel_ctx->world->accel_indices_size,
						&node,
						sizeof(node));
		return ;
	}
}

void
	world_accel(t_world *world)
{
	uint32_t	max_depth;
	t_accel_ctx	ctx;
	uint64_t	index;

	max_depth = (uint32_t) (8.0 + 1.3 * log2(world->primitives_count));
	ctx.next = 0;
	ctx.max = 0;
	index = 0;
	while (index < world->primitives_count) {
		world->accel_indices =
			vec_push_back(
				world->accel_indices,
				&world->accel_indices_size,
				&index, sizeof(*world->accel_indices));
		index++;
	}
}
