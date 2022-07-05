#include "world_impl.h"

#include "util.h"

static void
	world_best_split_axis(t_node_info *node, t_split *best, uint8_t axis)
{
	const t_edge	*edge;
	const t_edge	*end;
	t_split			current;
	uint32_t		prim_count[2];
	uint32_t		edge_count[2];
	int				in_bounds;

	edge = node->edges->edges[axis];
	end = edge + node->edges->count;
	current.axis = axis;
	prim_count[ACCEL_BELOW] = 0;
	prim_count[ACCEL_ABOVE] = node->edges->count / 2;
	while (edge != end)
	{
		current.offset = edge->offset;
		edge_count[EDGE_START] = 0;
		edge_count[EDGE_END] = 0;
		while (1)
		{
			edge_count[edge->type] += 1;
			edge += 1;
			if (edge == end || edge->offset != current.offset)
				break ;
		}
		in_bounds = (current.offset > xyz(node->bounds.min, axis)
			&& current.offset < xyz(node->bounds.max, axis));
		if (in_bounds && edge_count[EDGE_START] > 0)
		{
			current.cost = get_split_cost(node->bounds, &current, prim_count);
			if (current.cost < best->cost)
				*best = current;
		}
		prim_count[ACCEL_BELOW] += edge_count[EDGE_START];
		prim_count[ACCEL_ABOVE] -= edge_count[EDGE_END];
		if (in_bounds && edge_count[EDGE_END] > 0)
		{
			current.cost = get_split_cost(node->bounds, &current, prim_count);
			if (current.cost < best->cost)
				*best = current;
		}
	}
}

static void
	world_split_edges(t_node_info *node, t_split *split, uint8_t axis, size_t *counts)
{
	t_edge	*below;
	t_edge	*above;
	t_edge	*cur;
	t_edge	*end;
	int		side;

	below = node->edges[0].edges[axis];
	above = node->edges[1].edges[axis];
	cur = below;
	end = below + node->edges[0].count;
	while (cur != end)
	{
		side = world_axis_side(node->tree, split, cur->index);
		if (side <= 0)
		{
			*above = *cur;
			above += 1;
		}
		if (side >= 0)
		{
			*below = *cur;
			below += 1;
		}
		cur += 1;
	}
	counts[0] = below - node->edges[0].edges[axis];
	counts[1] = above - node->edges[1].edges[axis];
}

static void
	world_plant_tree(t_node_info *node)
{
	t_split		split;
	t_node_info	child;
	size_t		counts[2];

	if (node->depth > 0 && node->edges->count > RT_MAX_PRIMITIVES * 2)
	{
		split.cost = RT_HUGE_VAL;
		world_best_split_axis(node, &split, AXIS_X);
		world_best_split_axis(node, &split, AXIS_Y);
		world_best_split_axis(node, &split, AXIS_Z);
		if (split.cost < RT_HUGE_VAL)
		{
			world_split_edges(node, &split, AXIS_X, counts);
			world_split_edges(node, &split, AXIS_Y, counts);
			world_split_edges(node, &split, AXIS_Z, counts);
			node->edges[0].count = counts[0];
			node->edges[1].count = counts[1];
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
