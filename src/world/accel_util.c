#include "world_impl.h"
#include "util.h"
#include <math.h>

const t_primitive *get_primitive(const t_tree_info *info, size_t index) {
	/* rt_assert(index < info->prims_count, "out of bounds access in get_primitive"); */
	return (get_prim_const(info->world, info->prims[index].index));
}

static inline t_accel_node *get_node(t_node_info *node_info) {
	return (node_info->tree->world->accel_nodes + node_info->offset);
}

void interior_node_init(t_node_info *parent_info, const t_node_info *above_info, const t_split *split) {
	t_accel_node	*parent_node;

	parent_node = get_node(parent_info);
	parent_node->a.split = split->offset;
	parent_node->b.flags = split->axis;
	parent_node->b.above_child |= (above_info->offset << 2);
}

static void copy_indices(t_accel_node *node, t_tree_info *tree_info, t_tree_edges *edges) {
	const t_edge	*edge;
	size_t			index;
	size_t			offset;

	index = 0;
	offset = 0;
	while (index < edges->count)
	{
		edge = &edges->edges[AXIS_X][index];
		if (edge->type == EDGE_START) {
			node->a.inline_primitives[offset] = tree_info->prims[edge->index].index;
			++offset;
		}
		++index;
	}
}

static uint32_t push_back_indices(t_tree_info *tree_info, t_tree_edges *edges) {
	const t_edge	*edge;
	uint32_t		first_offset;
	size_t			index;
	uint32_t		edge_type;

	edge = &edges->edges[AXIS_X][0];
	first_offset = world_add_accel_index(tree_info->world, tree_info->prims[edge->index].index);
	edge_type = edge->type;
	index = 1;
	while (index < edges->count) {
		edge = &edges->edges[AXIS_X][index];
		if (edge->type == edge_type) {
			world_add_accel_index(tree_info->world, tree_info->prims[edge->index].index);
		}
		++index;
	}
	return (first_offset);
}

void leaf_node_init(t_node_info *node_info) {
	t_accel_node	*node;
	size_t			primitive_count;


	primitive_count = node_info->edges->count / 2;
	/* rt_assert(primitive_count < (1 << 30), "too many primitives in leaf_node_init"); */
	node = get_node(node_info);
	node->b.flags = 3;
	node->b.nprims |= ((uint32_t) primitive_count) << 2;
	if (primitive_count <= ACCEL_INLINE_PRIMS) {
		copy_indices(node, node_info->tree, node_info->edges);
	} else {
		node->a.primitive_offset = push_back_indices(node_info->tree, node_info->edges);
	}
}

static inline float calculate_cost(float total_sa, const float sub_sa[2], const uint32_t primitive_count[2]) {
	float	inv_total_sa;
	float	below_chance;
	float	above_chance;
	float	cost;

	inv_total_sa = 1.0 / total_sa;
	below_chance = sub_sa[ACCEL_BELOW] * inv_total_sa;
	above_chance = sub_sa[ACCEL_ABOVE] * inv_total_sa;
	cost = (RT_TRAVERSAL_COST +
			RT_INTERSECT_COST *
			(1.0 - (RT_EMPTY_BONUS * (primitive_count[ACCEL_BELOW] == 0 || primitive_count[ACCEL_ABOVE] == 0))) *
			(below_chance * primitive_count[ACCEL_BELOW] +
			 above_chance * primitive_count[ACCEL_ABOVE]));
	return (cost);
}

static void get_surface_areas(const t_bounds bounds, const float offset, const int axis, float out[2]) {
	t_vec	diagonal;
	int		other_axis0;
	int		other_axis1;

	diagonal = vec_sub(bounds.max, bounds.min);
	other_axis0 = (axis + 1) % 3;
	other_axis1 = (axis + 2) % 3;
	out[ACCEL_BELOW] = 2.0 *
		(xyz(diagonal, other_axis0) * xyz(diagonal, other_axis1) +
		 (offset  - xyz(bounds.min, axis)) *
		 (xyz(diagonal, other_axis0) + xyz(diagonal, other_axis1)));
	out[ACCEL_ABOVE] = 2.0 *
		(xyz(diagonal, other_axis0) * xyz(diagonal, other_axis1) +
		 (xyz(bounds.max, axis) - offset) *
		 (xyz(diagonal, other_axis0) + xyz(diagonal, other_axis1)));
}

float get_split_cost(const t_bounds bounds, const t_split *split, const uint32_t primitive_counts[2]) {
	float		surface_areas[2];

	get_surface_areas(bounds, split->offset, split->axis, surface_areas);
	return (calculate_cost(bounds_surf(bounds), surface_areas, primitive_counts));
}

uint32_t world_max_depth(size_t prims_count) {
	return (8.0 + 1.3 * log2((float) prims_count));
}

uint32_t	new_node(t_world *world) {
	t_accel_node new_node;
	return (world_add_accel_node(world, &new_node));
}

int world_axis_side(const t_tree_info *tree, const t_split *split, uint32_t index) {
	t_bounds	prim_bounds;
	float		min_offset;
	float		max_offset;
	float		axis_offset;

	prim_bounds = tree->prims[index].bounds;
	max_offset = xyz(prim_bounds.max, split->axis);
	min_offset = xyz(prim_bounds.min, split->axis);
	axis_offset = split->offset;
	if (max_offset >= axis_offset && min_offset >= axis_offset)
		return (1);
	else if (max_offset <= axis_offset && min_offset <= axis_offset)
		return (-1);
	return (0);
}
