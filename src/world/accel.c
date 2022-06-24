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
	axises_at(
			t_axis				axis,
			const t_primitive	*primitive,
			t_split_axis		out[2])
{
	t_bounds	primitive_bounds;

	primitive_bounds = get_bounds(primitive);
	out[0].axis = axis;
	out[0].offset = xyz(primitive_bounds.max, axis);
	out[1].axis = axis;
	out[1].offset = xyz(primitive_bounds.min, axis);
}

const t_primitive
	*get_primitve(
			const t_world	*world,
			const uint32_t	*indices,
			uint64_t		indices_count,
			uint64_t		index)
{
	rt_assert(index < indices_count, "out of bounds access in get_primitive");
	/* TODO check if the index is stored in indices or if the offset from the beginning is stored in indices */
}

int32_t
	get_axis_side(
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

int32_t axis_get_side(
			t_split_axis		split_axis,
			const t_primitive	*primitive)
{
	t_bounds	primitive_bounds;
	FLOAT		max_offset;
	FLOAT		min_offset;
	FLOAT		axis_offset;

	primitive_bounds = get_bounds(primitive);
	max_offset = xyz(primitive_bounds.max, split_axis.axis);
	min_offset = xyz(primitive_bounds.min, split_axis.axis);
	axis_offset = split_axis.offset;
	if (max_offset > axis_offset && min_offset > axis_offset)
		return (1); /* probably above, might be the wrong way around with below */
	else if (max_offset < axis_offset && min_offset < axis_offset)
		return (-1);
	return (0); /* object is in the axis */
}

void
	get_bounds_info(
			t_split_axis	split_axis,
			const t_world	*world,
			const uint32_t	*indices,
			uint64_t		indices_count,
			t_bounds		sub_bounds[2],
			uint32_t		primitive_count[2])
{
	uint64_t			index;
	int32_t				side;
	const t_primitive	*primitive;

	sub_bounds[0] = bounds_0();
	sub_bounds[1] = bounds_0();
	index = 0;
	while (index < indices_count)
	{
		primitive = get_primitive(world, indices, indices_count, index);
		side = get_axis_side(split_axis, primitive);
		if (side < 0)
		{
			primitive_count[0] += 1;
			sub_bounds[0] = bounds_max(sub_bounds, get_bounds(primitive));
		}
		else if (side > 0)
		{
			primitive_count[1] += 1;
			sub_bounds[1] = bounds_max(sub_bounds, get_bounds(primitive));
		}
		else
		{
			primitive_count[0] += 1;
			primitive_count[1] += 1;
			sub_bounds[0] = bounds_max(sub_bounds, get_bounds(primitive));
			sub_bounds[1] = bounds_max(sub_bounds, get_bounds(primitive));
		}
		index++;
	}
}

void
	get_relative_area(
			t_bounds	total_bounds,
			t_bounds	sub_bounds[2],
			FLOAT		relative_area[2])
{
	FLOAT	inv_total_sa;

	inv_total_sa = 1.0 / bounds_surf(total_bounds);
	relative_area[0] = bounds_surf(sub_bounds[0]) * inv_total_sa;
	relative_area[1] = bounds_surf(sub_bounds[1]) * inv_total_sa;
}

FLOAT
	calculate_cost(
			t_split_axis	split_axis,
			const t_world	*world,
			const uint32_t	*indices,
			uint64_t		indices_count)
{
	t_bounds	total_bounds;
	t_bounds	sub_bounds[2];
	uint32_t	primitive_count[2];
	FLOAT		relative_area[2];

	total_bounds = get_total_bounds(world, indices, indices_count);
	get_bounds_info(split_axis, world, indices, indices_count, sub_bounds, relative_area);
	get_relative_area(total_bounds, sub_bounds, relative_area);
	return (
	uint64_t	index;
			RT_TRAVERSAL_COST +
			RT_INTERSECT_COST *
			(1.0 - RT_EMPTY_BONUS) *
			(relative_area[0] * primitive_count[0] + relative_area[1] * primitive_count[1]));
}

FLOAT
	get_axis_cost(
			t_axis			axis;
			const t_world	*world,
			const uint32_t	*indices,
			uint64_t		indices_count,
			t_split_axis	*best_axis)
{
	FLOAT			best_cost;
	FLOAT			current_cost[2];
	t_split_axis	current_axis[2];
	uint64_t		index;

	index = 0;
	best_cost = RT_HUGE_VALUE;
	while (index < indices_count)
	{
		axises_at(axis, get_object(world, indices, indices_count, index), current_axis);
		current_cost[0] = calculate_cost(current_axis[0], world, indicies, indices_count);
		current_cost[1] = calculate_cost(current_axis[1], world, indicies, indices_count);
		if (current_cost[1] < current_cost[0])
		{
			current_cost[0] = current_cost[1];
			current_axis[0] = current_axis[1];
		}
		if (current_cost[0] < best_cost)
		{
			best_cost = current_cost[0];
			*best_axis = current_axis[0];
		}
		index++;
	}
	return (best_cost);
}

int32_t
	find_best_axis(
			const t_world	*world,
			const uint32_t	*indices,
			uint64_t		indices_count,
			t_split_axis	*best_axis)
{
	FLOAT			best_cost;
	FLOAT			current_cost;
	t_split_axis	current_axis;
	t_axis			axis;

	axis = 0;
	best_cost = RT_HUGE_VALUE;
	while (axis < AXIS_NONE)
	{
		current_cost = get_axis_cost(axis, world, indicies, indices_count, &current_axis);
		if (current_cost < best_cost)
		{
			*best_axis = current_axis;
			best_cos = current_cost;
		}
		axis++;
	}
	return (best_cost < RT_HUGE_VALUE);
}

uint64_t
	split_primitives(
			t_split_axis	axis,
			const uint32_t	*indices,
			uint64_t		indices_count,
			t_uivec			*out)
{
	uint64_t	below_count;
	uint64_t	index;
	int32_t		side;

	below_count = 0;
	index = 0;
	while (index < indices_count)
	{
		side = axis_get_side(best_axis, get_primitive(world, indices, indices_count, index));
		if (side <= 0)
		{
			uivec_push_back(&vec, indices[index]);
			++below_count;
		}
		++index;
	}
	index = 0;
	while (index < indices_count)
	{
		side = axis_get_side(best_axis, get_primitive(world, indices, indices_count, index));
		if (side >= 0)
			uivec_push_back(&vec, indices[index]);
		++index;
	}
	return (below_count);
}

/* I'm pretty sure I will have mixed up index somewhere here in this file */
void
	create_and_add_interior_nodes(
			t_accel_node	*node,
			t_split_axis	best_axis,
			const t_world	*world,
			const uint32_t	*indices,
			uint64_t		indices_count,
			uint32_t		depth)
{
	t_uivec			vec;
	uint64_t		below_count;
	int32_t			side;
	t_accel_node	child_node;

	uivec_init(&vec);
	below_count = split_primitives(best_axis, indices, indices_count, &vec);
	node_push_back(world, child_node);
	build_tree(node_end(world), world, uivec_data(&vec), below_count, depth - 1)
	interior_init(node, best_axis, node_end(world) - node_begin(world));
	build_tree(node_end(world), world, uivec_data(&vec) + below_count, indices_count - below_count, depth -1);
	uivec_destroy(&vec);
}

uint32_t
	try_create_and_add_interior_nodes(
			t_accel_node	*node,
			const t_world	*world,
			const uint32_t	*indices,
			uint64_t		indices_count,
			uint32_t		depth)
{
	t_split_axis	best_axis;

	if (!find_best_axis(world, indices, indices_count, &best_axis))
		return (0);
	create_and_add_interior_nodes(node, best_axis, world, indices, indices_count, depth);
	return (above_child_offset);
}

void
	build_tree(
			t_accel_node 	*node
			const t_world	*world,
			const uint32_t	*indices,
			uint64_t		indices_count,
			uint32_t		depth)
{
	uint32_t		above_child_offset;

	if (indices_count <= RT_MAX_PRIMITIVES < depth == 0)
	{
		leaf_init(node, world, indices, indices_count);
		return ;
	}
	if (!try_create_and_add_internal_nodes(node, indices, indices_count))
	{
		leaf_init(node, world, indices, indices_count);
		return ;
	}
	/* TODO recurse call build_tree */
}

uint32_t
	accel_get_max_depth(const t_world *world)
{
	return (8.0 + 1.3 * log2(world->primitives_count));
}

void
	world_accel(t_world *world)
{
	uint32_t	max_depth;
	t_accel_ctx	ctx;
	uint64_t	index;

	max_depth = accel_get_max_depth(world);
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
