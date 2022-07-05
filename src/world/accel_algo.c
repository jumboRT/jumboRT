#include "world_impl.h"

#include "util.h"

static void
	world_best_split_axis(t_node_info *node, t_split *best, uint8_t axis)
{
	const t_edge	*edge;
	t_split			current;
	size_t			index;
	t_view			edges;

	index = 0;
	edges = node->edges->edges[axis].view;
	current.axis = axis;
	current.prim_count[ACCEL_BELOW] = 0;
	current.prim_count[ACCEL_ABOVE] = view_size(edges) / 2;
	while (index < view_size(edges))
	{
		edge = view_get(edges, index);
		if (edge->type == EDGE_END)
			current.prim_count[ACCEL_ABOVE] -= 1;
		if (edge->offset > xyz(node->bounds.min, axis)
			&& edge->offset < xyz(node->bounds.max, axis))
		{
			current.offset = edge->offset;
			current.cost = get_split_cost(node->bounds, &current);
			if (current.cost < best->cost)
				*best = current;
		}
		if (edge->type == EDGE_START)
			current.prim_count[ACCEL_BELOW] += 1;
		index += 1;
	}
}

static int
	world_best_split(t_node_info *node, t_split *split)
{
	split->cost = RT_HUGE_VAL;

	world_best_split_axis(node, split, AXIS_X);
	world_best_split_axis(node, split, AXIS_Y);
	world_best_split_axis(node, split, AXIS_Z);
	return (split->cost < RT_HUGE_VAL);
}

static void
	world_split_edges(t_tree_info *tree, t_tree_edges *edges, t_split *split, uint8_t axis)
{
	size_t			index;
	size_t			below;
	const t_edge	*edge;
	int				side;

	index = 0;
	below = 0;
	vector_clear(&(edges + 1)->edges[axis]);
	while (index < view_size(edges->edges[axis].view))
	{
		edge = view_get(edges->edges[axis].view, index);
		side = world_axis_side(tree, split, edge->index);
		if (side <= 0)
		{
			vector_push(&(edges + 1)->edges[axis], edge);
		}
		if (side >= 0)
		{
			*(t_edge *) view_get(edges->edges[axis].view, below) = *edge;
			below += 1;
		}
		index += 1;
	}
	edges->edges[axis].view.size = sizeof(t_edge) * below;
}

static void
	world_plant_tree(t_node_info *node)
{
	t_split		split;
	t_node_info	child;

	if (node->depth > 0 && view_size(node->edges->edges[0].view) > RT_MAX_PRIMITIVES * 2)
	{
		if (world_best_split(node, &split))
		{
			world_split_edges(node->tree, node->edges, &split, AXIS_X);
			world_split_edges(node->tree, node->edges, &split, AXIS_Y);
			world_split_edges(node->tree, node->edges, &split, AXIS_Z);
			child.tree = node->tree;
			child.offset = new_node(node->tree->world);
			child.depth = node->depth - 1;
			child.edges = node->edges + 1;
			child.bounds = bounds(node->bounds.min, vec_set(node->bounds.max, split.axis, split.offset));
			world_plant_tree(&child);
			child.offset = new_node(node->tree->world);
			child.edges = node->edges;
			child.bounds = bounds(vec_set(node->bounds.min, split.axis, split.offset), node->bounds.max);
			interior_node_init(node, &child, &split);
			world_plant_tree(&child);
			return ;
		}
	}
	leaf_node_init(node);
}

void
	world_accel(t_world *world)
{
	t_tree_info		tree;
	t_node_info		node;
	t_accel_node	root;

	world_add_accel_node(world, &root);
	world_info_create(&tree, &node, world);
	world_info_init(&tree, &node, world);
	world_plant_tree(&node);
	world_info_destroy(&tree, &node);
}
