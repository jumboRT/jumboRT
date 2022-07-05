#include "world_impl.h"
#include "util.h"
#include <math.h>

static inline const t_prim_info *get_prim_info(const t_tree_info *tree_info, uint32_t index) {
	return (view_get(tree_info->prims.view, index));
}

static inline const t_primitive *get_primitive_world(const t_world *world, size_t index) {
	rt_assert(index * RT_PRIMITIVE_ALIGN < world->primitives_size, "out of bounds access in get_primitive_world");
	return ((const t_primitive *)
			((const char *) world->primitives + (index * RT_PRIMITIVE_ALIGN)));
}

const t_primitive *get_primitive(const t_tree_info *info, size_t index) {
	rt_assert(index < view_size(info->prims.view), "out of bounds access in get_primitive");
	return (get_primitive_world(info->world, ((const t_prim_info *) view_get(info->prims.view, index))->index));
}

static inline t_vec get_vertex(const t_world *world, size_t index) {
	return (world->vertices[index].pos);
}

static inline t_bounds get_bounds_triangle(const t_world *world, const t_shape_triangle *triangle) {
	t_vec	points[3];

	points[0] = get_vertex(world, triangle->a);
	points[1] = get_vertex(world, triangle->b);
	points[2] = get_vertex(world, triangle->c);
	return (bounds(
				vec_min(points[0], vec_min(points[1], points[2])),
				vec_max(points[0], vec_max(points[1], points[2]))));
}

static inline t_bounds get_bounds_sphere(const t_shape_sphere *sphere) {
	return (bounds(
				vec_sub(sphere->pos, vec(sphere->radius, sphere->radius, sphere->radius)),
				vec_add(sphere->pos, vec(sphere->radius, sphere->radius, sphere->radius))));
}

static inline t_accel_node *get_node(t_node_info *node_info) {
	return (node_info->tree->world->accel_nodes + node_info->offset);
}

t_bounds get_bounds(const t_world *world, const t_primitive *primitive) {
	if (primitive->shape_type == RT_SHAPE_TRIANGLE) {
		return (get_bounds_triangle(world, (const t_shape_triangle *) primitive));
	} else if (primitive->shape_type == RT_SHAPE_SPHERE) {
		return (get_bounds_sphere((const t_shape_sphere *) primitive));
	}
	rt_assert(0, "unimplemented shape in get_bounds");
	return (bounds_0());
}

void interior_node_init(t_node_info *parent_info, const t_node_info *above_info, const t_split *split) {
	t_accel_node	*parent_node;

	parent_node = get_node(parent_info);
	parent_node->a.split = split->offset;
	parent_node->b.flags = split->axis;
	parent_node->b.above_child |= (above_info->offset << 2);
}

static uint32_t push_back_indices(t_tree_info *tree_info, t_view edges) {
	const t_edge	*edge;
	uint32_t		first_offset;
	size_t			index;
	uint32_t		edge_type;

	edge = view_get(edges, 0);
	first_offset = world_add_accel_index(tree_info->world, get_prim_info(tree_info, edge->index)->index);
	edge_type = edge->type;
	index = 1;
	while (index < view_size(edges)) {
		edge = view_get(edges, index);
		if (edge->type == edge_type) {
			world_add_accel_index(tree_info->world, get_prim_info(tree_info, edge->index)->index);
		}
		++index;
	}
	return (first_offset);
}

void leaf_node_init(t_node_info *node_info) {
	t_accel_node	*node;
	size_t			primitive_count;
	t_view			edges;

	edges = node_info->edges->edges[0].view;
	primitive_count = view_size(edges) / 2;
	rt_assert(primitive_count < (1 << 30), "too many primitives in leaf_node_init");
	node = get_node(node_info);
	node->b.flags = 3;
	node->b.nprims |= ((uint32_t) primitive_count) << 2;
	if (primitive_count == 0) {
		node->a.one_primitive = 0;
	} else if (primitive_count == 1) {
		node->a.one_primitive = get_prim_info(node_info->tree, ((const t_edge *) view_get(edges, 0))->index)->index;
	} else {
		node->a.primitive_ioffset = push_back_indices(node_info->tree, edges);
	}
}

static inline FLOAT calculate_cost(FLOAT total_sa, const FLOAT sub_sa[2], const uint32_t primitive_count[2]) {
	FLOAT	inv_total_sa;
	FLOAT	below_chance;
	FLOAT	above_chance;
	FLOAT	cost;

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

static void get_surface_areas(const t_bounds bounds, const FLOAT offset, const int axis, FLOAT out[2]) {
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

FLOAT get_split_cost(const t_bounds bounds, const t_split *split) {
	FLOAT		surface_areas[2];
	uint32_t	primitive_counts[2];

	get_surface_areas(bounds, split->offset, split->axis, surface_areas);
	primitive_counts[ACCEL_ABOVE] = split->prim_count[ACCEL_ABOVE];
	primitive_counts[ACCEL_BELOW] = split->prim_count[ACCEL_BELOW];
	return (calculate_cost(bounds_surf(bounds), surface_areas, primitive_counts));
}

uint32_t world_max_depth(const t_world *world) {
	return (8.0 + 1.3 * log2(world->primitives_count));
}

uint32_t	new_node(t_world *world) {
	t_accel_node new_node;
	return (world_add_accel_node(world, &new_node));
}

/* TODO: don't use get_bounds() */
t_bounds world_bounds(const t_world *world) {
	t_bounds			result;
	t_bounds			prim_bounds;
	size_t				index;
	const t_primitive	*primitive;

	primitive = get_primitive_world(world, 0);
	result = get_bounds(world, primitive);
	index = world_primitive_size(primitive->shape_type) / RT_PRIMITIVE_ALIGN;
	while (index < world->primitives_size / RT_PRIMITIVE_ALIGN) {
		primitive = get_primitive_world(world, index);
		prim_bounds = get_bounds(world, primitive);
		result = bounds_max(result, prim_bounds);
		index += world_primitive_size(primitive->shape_type) / RT_PRIMITIVE_ALIGN;
	}
	return (result);
}

/* TODO: don't use get_bounds() */
int world_axis_side(const t_tree_info *tree, const t_split *split, uint32_t index) {
	t_bounds	prim_bounds;
	FLOAT		min_offset;
	FLOAT		max_offset;
	FLOAT		axis_offset;

	prim_bounds = get_prim_info(tree, index)->bounds;
	max_offset = xyz(prim_bounds.max, split->axis);
	min_offset = xyz(prim_bounds.min, split->axis);
	axis_offset = split->offset;
	if (max_offset >= axis_offset && min_offset >= axis_offset)
		return (1);
	else if (max_offset <= axis_offset && min_offset <= axis_offset)
		return (-1);
	return (0);
}
