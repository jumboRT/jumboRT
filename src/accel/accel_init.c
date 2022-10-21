#include "accel_impl.h"
#include "world_impl.h"

static inline t_accel_node
	*get_node(t_node_info *node_info) {
	return (node_info->tree->world->accel_nodes + node_info->offset);
}

static void
	copy_indices(t_accel_node *node, t_tree_info *tree_info,
			t_tree_edges *edges)
{
	const t_edge	*edge;
	size_t			index;
	size_t			offset;

	index = 0;
	offset = 0;
	while (index < edges->count)
	{
		edge = &edges->edges[AXIS_X][index];
		if (edge->type == EDGE_START)
		{
			node->u_a.inline_primitives[offset]
				= tree_info->prims[edge->index].index;
			++offset;
		}
		++index;
	}
}

static uint32_t
	push_back_indices(t_tree_info *tree_info, t_tree_edges *edges)
{
	const t_edge	*edge;
	uint32_t		first_offset;
	size_t			index;
	uint32_t		edge_type;

	edge = &edges->edges[AXIS_X][0];
	first_offset = world_add_accel_index(tree_info->world,
			tree_info->prims[edge->index].index);
	edge_type = edge->type;
	index = 1;
	while (index < edges->count)
	{
		edge = &edges->edges[AXIS_X][index];
		if (edge->type == edge_type)
		{
			world_add_accel_index(tree_info->world,
				tree_info->prims[edge->index].index);
		}
		++index;
	}
	return (first_offset);
}

void
	interior_node_init(t_node_info *parent_info, const t_node_info *above_info,
			const t_split *split) {
	t_accel_node	*parent_node;

	parent_node = get_node(parent_info);
	parent_node->u_a.split = split->offset;
	parent_node->u_b.flags = split->axis;
	parent_node->u_b.above_child |= (above_info->offset << 2);
}

void
	leaf_node_init(t_node_info *node_info)
{
	t_accel_node	*node;
	size_t			primitive_count;

	primitive_count = node_info->edges->count / 2;
	node = get_node(node_info);
	node->u_b.flags = 3;
	node->u_b.nprims |= ((uint32_t) primitive_count) << 2;
	if (primitive_count <= ACCEL_INLINE_PRIMS)
		copy_indices(node, node_info->tree, node_info->edges);
	else
		node->u_a.primitive_offset = push_back_indices(node_info->tree,
				node_info->edges);
}
