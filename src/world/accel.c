#include "world.h"

#include "aabb.h"
#include "util.h"
#include "vector.h"
#include "world_impl.h"
#include <math.h>
#include <stddef.h>


#include <stdio.h>

void build_tree(
			uint32_t		parent_offset,
			t_world			*world,
			t_vector		*indices,
			uint32_t		depth);

t_vec
	get_vertex(
			const t_world	*world,
			uint32_t		index)
{
	rt_assert(index < world->vertices_count, "out of bounds access in get_vertex");
	return (world->vertices[index].pos);
}

t_bounds
	get_bounds_triangle(
			const t_world			*world,
			const t_shape_triangle	*triangle)
{
	t_vec	points[3];

	points[0] = get_vertex(world, triangle->a);
	points[1] = get_vertex(world, triangle->b);
	points[2] = get_vertex(world, triangle->c);
	return (bounds(
				vec_min(vec_min(points[0], points[1]), points[2]),
				vec_max(vec_max(points[0], points[1]), points[2])));
}

t_bounds
	get_bounds_sphere(
			const t_world			*world,
			const t_shape_sphere	*sphere)
{
	(void) world;
	return (bounds(
				vec_sub(sphere->pos, vec(sphere->radius, sphere->radius, sphere->radius)),
				vec_add(sphere->pos, vec(sphere->radius, sphere->radius, sphere->radius))));
}

t_bounds
	get_bounds(
		const t_world		*world,
		const t_primitive	*primitive)
{
	if (primitive->shape_type == RT_SHAPE_TRIANGLE)
		return (get_bounds_triangle(world, (const t_shape_triangle *) primitive));
	return (get_bounds_sphere(world, (const t_shape_sphere *) primitive));
}

const t_primitive
	*get_primitive(
			const t_world	*world,
			uint32_t		offset)
{
	rt_assert(offset * RT_PRIMITIVE_ALIGN < world->primitives_size, "out of bounds access in get_primitive");
	rt_assert((offset % RT_PRIMITIVE_ALIGN) == 0, "misaligned access in get_primitive");
	return (
			(const t_primitive *)
			((const char *) world->primitives +
			(offset * RT_PRIMITIVE_ALIGN)));
}

int32_t get_axis_side(
			const t_world		*world,
			t_split_axis		split_axis,
			const t_primitive	*primitive)
{
	t_bounds	primitive_bounds;
	FLOAT		max_offset;
	FLOAT		min_offset;
	FLOAT		axis_offset;

	primitive_bounds = get_bounds(world, primitive);
	max_offset = xyz(primitive_bounds.max, split_axis.axis);
	min_offset = xyz(primitive_bounds.min, split_axis.axis);
	axis_offset = split_axis.offset;
	if (max_offset >= axis_offset && min_offset >= axis_offset)
		return (1); /* probably above, might be the wrong way around with below */
	else if (max_offset <= axis_offset && min_offset <= axis_offset)
		return (-1);
	return (0); /* object is in the axis */
}

void
	get_bounds_info(
			t_split_axis	split_axis,
			const t_world	*world,
			t_vector		*indices,
			t_bounds		sub_bounds[2],
			uint32_t		primitive_count[2])
{
	uint64_t			index;
	int32_t				side;
	const t_primitive	*primitive;

	sub_bounds[0] = bounds_0();
	sub_bounds[1] = bounds_0();
	index = 0;
	while (index < vector_size(indices))
	{
		primitive = get_primitive(world, *(uint32_t *) vector_at(indices, index));
		side = get_axis_side(world, split_axis, primitive);
		if (side < 0)
		{
			primitive_count[0] += 1;
			sub_bounds[0] = bounds_max(sub_bounds[0], get_bounds(world, primitive));
		}
		else if (side > 0)
		{
			primitive_count[1] += 1;
			sub_bounds[1] = bounds_max(sub_bounds[1], get_bounds(world, primitive));
		}
		else
		{
			primitive_count[0] += 1;
			primitive_count[1] += 1;
			sub_bounds[0] = bounds_max(sub_bounds[0], get_bounds(world, primitive));
			sub_bounds[1] = bounds_max(sub_bounds[1], get_bounds(world, primitive));
		}
		index++;
	}
}

t_bounds
	get_total_bounds(
			const t_world	*world,
			t_vector		*indices)
{
	const t_primitive	*primitive;
	uint64_t			index;
	t_bounds			total_bounds;

	rt_assert(vector_size(indices) != 0, "empty indices");
	if (vector_size(indices) == 0)
		return (bounds_0());
	index = 1;
	total_bounds = get_bounds(world, get_primitive(world, *(uint32_t *) vector_at(indices, index)));
	while (index < vector_size(indices))
	{
		primitive = get_primitive(world, *(uint32_t *) vector_at(indices, index));
		total_bounds = bounds_max(total_bounds, get_bounds(world, primitive));
		index++;
	}
	return (total_bounds);
}

/*
 * primitive_counts[0] is below
 */
FLOAT
	calculate_cost(
			FLOAT			total_sa,
			FLOAT			below_sa,
			FLOAT			above_sa,
			const uint32_t	primitive_counts[2])
{
	FLOAT	inv_total_sa;
	FLOAT	below_chance;
	FLOAT	above_chance;
	FLOAT	cost;

	rt_assert(fabs(total_sa) >= 0.00001, "total sa almost zero");
	rt_assert(fabs(below_sa) >= 0.00001, "below sa almost zero");
	rt_assert(fabs(above_sa) >= 0.00001, "above sa almost zero");

	inv_total_sa = 1.0 / total_sa;
	below_chance = below_sa * inv_total_sa;
	above_chance = above_sa * inv_total_sa;

	cost = (RT_TRAVERSAL_COST +
			RT_INTERSECT_COST *
			(1.0 - (RT_EMPTY_BONUS * (primitive_counts[0] == 0 || primitive_counts[1] == 1))) *
			(below_chance * primitive_counts[0] +
			 above_chance * primitive_counts[1]));
	return (cost);
}

FLOAT
	axis_cost_at_offset(
			t_axis			axis,
			t_bounds		total_bounds,
			const uint32_t	primitive_counts[2],
			FLOAT			offset)
{
	t_vec	diagonal;
	t_axis	other_axis0;
	t_axis	other_axis1;
	FLOAT	below_sa;
	FLOAT	above_sa;

	diagonal = vec_sub(total_bounds.max, total_bounds.min);
	other_axis0 = (axis + 1) % 3;
	other_axis1 = (axis + 2) % 3;
	above_sa = 2.0 *
		(xyz(diagonal, other_axis0) * xyz(diagonal, other_axis1) +
		 (offset  - xyz(total_bounds.min, axis)) *
		 (xyz(diagonal, other_axis0) + xyz(diagonal, other_axis1)));
	below_sa = 2.0 *
		(xyz(diagonal, other_axis0) * xyz(diagonal, other_axis1) +
		 (xyz(total_bounds.max, axis) - offset) *
		 (xyz(diagonal, other_axis0) + xyz(diagonal, other_axis1)));
	return (calculate_cost(
				bounds_surf(total_bounds),
				below_sa,
				above_sa,
				primitive_counts));
}

FLOAT
	get_best_offset(
			t_axis		axis,
			t_vector	*edges,
			t_bounds	total_bounds,
			FLOAT		*best_cost)
{
	uint64_t		index;
	FLOAT			current_cost;
	FLOAT			best_offset;
	const t_edge	*current;
	uint32_t		primitive_counts[2];

	best_offset = RT_HUGE_VAL; /* It doesn't really matter what this is initialized to */
	index = 0;
	*best_cost = RT_HUGE_VAL;
	primitive_counts[0] = 0;
	primitive_counts[1] = vector_size(edges) / 2;
	printf("total_bounds %f %f %f | %f %f %f\n", x(total_bounds.min), y(total_bounds.min), z(total_bounds.min), x(total_bounds.max), y(total_bounds.max), z(total_bounds.max));
	while (index < vector_size(edges))
	{
		current = vector_at(edges, index);
		if (current->type == EDGE_END)
			primitive_counts[1] -= 1;
		if (current->offset > xyz(total_bounds.min, axis)
				&& current->offset < xyz(total_bounds.max, axis))
		{
			current_cost = axis_cost_at_offset(axis, total_bounds, primitive_counts, current->offset);
			if (current_cost < *best_cost)
			{
				*best_cost = current_cost;
				best_offset = current->offset;
			}
		}
		if (current->type == EDGE_START)
			primitive_counts[0] += 1;
		index++;
	}
	return (best_offset);
}

t_vector
	get_all_edges(
			const t_world	*world,
			t_vector		*indices,
			t_axis			axis)
{
	uint64_t	index;
	t_vector	result;
	t_vec		bounds[2];
	t_bounds	current_bounds;
	t_edge		edge;

	index = 0;
	vector_init(&result, sizeof(t_edge));
	while (index < vector_size(indices))
	{
		current_bounds = get_bounds(world, get_primitive(world, *(uint32_t *) vector_at(indices, index)));
		bounds[0] = current_bounds.max;
		bounds[1] = current_bounds.min;
		if (xyz(current_bounds.min, axis) < xyz(current_bounds.max, axis))
		{
			bounds[1] = current_bounds.max;
			bounds[0] = current_bounds.min;
		}
		edge.offset = xyz(bounds[0], axis);
		edge.type = EDGE_END;
		vector_push_back(&result, &edge);
		edge.offset = xyz(bounds[1], axis);
		edge.type = EDGE_START;
		vector_push_back(&result, &edge);
		index++;
	}
	return (result);
}

int32_t
	cmp_edge(const void *edge0_ptr, const void *edge1_ptr, void *ctx_ptr)
{
	const t_edge	*edge0; 
	const t_edge	*edge1; 

	(void) ctx_ptr;
	edge0 = edge0_ptr;
	edge1 = edge1_ptr;
	return ((edge0->offset < edge1->offset) - (edge0->offset > edge1->offset));
}

int32_t
	find_best_axis(
			const t_world	*world,
			t_vector		*indices,
			t_split_axis	*best_axis)
{
	FLOAT			best_cost;
	FLOAT			current_cost;
	t_vector		edges;
	t_axis			axis;
	t_bounds		total_bounds;
	FLOAT			current_offset;

	axis = AXIS_X;
	total_bounds = get_total_bounds(world, indices);
	best_cost = RT_HUGE_VAL;
	while (axis < AXIS_NONE)
	{
		edges = get_all_edges(world, indices, axis);
		vector_sort(&edges, cmp_edge, &axis);
		current_offset = get_best_offset(axis, &edges, total_bounds, &current_cost); 
		if (current_cost < best_cost)
		{
			best_cost = current_cost;
			best_axis->axis = axis;
			best_axis->offset = current_offset;
		}
		vector_destroy(&edges, NULL);
		++axis;
	}
	/* fprintf(stderr, "best axis %d with cost %f and offset %f\n", (int) best_axis->axis, best_cost, best_axis->offset); */
	return (best_cost < RT_HUGE_VAL);
}

void
	split_primitives(
			const t_world	*world,
			t_split_axis	axis,
			t_vector		*indices,
			t_vector		sub_indices[2])
{
	uint64_t	index;
	int32_t		side;

	index = 0;
	vector_init(&sub_indices[0], sizeof(uint32_t));
	vector_init(&sub_indices[1], sizeof(uint32_t));
	while (index < vector_size(indices))
	{
		side = get_axis_side(world, axis, get_primitive(world, *(uint32_t *) vector_at(indices, index)));
		if (side <= 0)
		{
			vector_push_back(&sub_indices[0], vector_at(indices, index));
		}
		if (side >= 0)
		{
			vector_push_back(&sub_indices[1], vector_at(indices, index));
		}
		++index;
	}
}

void
	interior_node_init(
			uint32_t		offset,
			t_world			*world,
			t_split_axis	axis,
			uint32_t		above_child)
{
	t_accel_node	*node;

	node = world->accel_nodes + offset;
	node->a.split = axis.offset;
	node->b.flags = axis.axis;
	node->b.above_child |= (above_child << 2);
}

uint32_t
	push_back_primitives(
			t_world		*world,
			t_vector	*indices)
{
	uint32_t	first_index;
	uint64_t	index;

	rt_assert(vector_size(indices) >= 1, "empty indices vector");
	first_index = world_add_accel_index(world, (uint32_t *) vector_at(indices, 0));
	index = 1;
	while (index < vector_size(indices))
	{
		world_add_accel_index(world, (uint32_t *) vector_at(indices, index));
		index++;
	}
	return (first_index);
}

void
	leaf_node_init(
			uint32_t		offset,
			t_world			*world,
			t_vector		*indices)
{
	uint32_t		primitive_count;
	t_accel_node	*node;

	node = world->accel_nodes + offset;
	primitive_count = vector_size(indices);
	node->b.flags = 3;
	node->b.nprims |= (primitive_count << 2);
	if (primitive_count == 0)
		node->a.one_primitive = 0;
	else if (primitive_count == 1)
		node->a.one_primitive = *(uint32_t *) vector_at(indices, 0);
	else
	{
		node->a.one_primitive = push_back_primitives(world, indices);
	}
}

/* I'm pretty sure I will have mixed up index somewhere here in this file */
void
	create_and_add_interior_nodes(
			uint32_t		offset,
			t_split_axis	best_axis,
			t_world			*world,
			t_vector		*indices,
			uint32_t		depth)
{
	t_vector		sub_indices[2];
	t_accel_node	child_node;
	uint32_t		tmp_offset;

	split_primitives(world, best_axis, indices, sub_indices);
	tmp_offset = world_add_accel_node(world, &child_node);
	build_tree(
			tmp_offset, /* TODO check, this might be one past the element we want */
			world,
			&sub_indices[0],
			depth - 1);
	tmp_offset = world_add_accel_node(world, &child_node);
	interior_node_init(offset, world, best_axis, tmp_offset);
	build_tree(
			tmp_offset, /* TODO check, this might be one past the element we want */
			world,
			&sub_indices[1],
			depth -1);
	vector_destroy(&sub_indices[0], NULL);
	vector_destroy(&sub_indices[1], NULL);
}

uint32_t
	try_create_and_add_interior_nodes(
			uint32_t		offset,
			t_world			*world,
			t_vector		*indices,
			uint32_t		depth)
{
	t_split_axis	best_axis;

	if (!find_best_axis(world, indices, &best_axis))
		return (0);
	create_and_add_interior_nodes(offset, best_axis, world, indices, depth);
	return (1);
}

void
	build_tree(
			uint32_t	parent_offset,
			t_world		*world,
			t_vector	*indices,
			uint32_t	depth)
{
	if (vector_size(indices) <= RT_MAX_PRIMITIVES || depth == 0)
	{
		leaf_node_init(parent_offset, world, indices);
		return ;
	}
	if (!try_create_and_add_interior_nodes(parent_offset, world, indices, depth))
	{
		leaf_node_init(parent_offset, world, indices);
		return ;
	}
}

uint32_t
	accel_get_max_depth(const t_world *world)
{
	fprintf(stderr, "%f vs %f\n", 8.0 + 1.3 * log(world->primitives_count), 8.0 + 1.3 * log2(world->primitives_count));
	return (4.0);
	return (8.0 + 1.3 * log2(world->primitives_count));
}

void
	world_accel(t_world *world)
{
	const t_primitive	*primitive;
	t_vector			all_indices;
	uint64_t			index;
	t_accel_node		root;

	index = 0;
	vector_init(&all_indices, sizeof(uint32_t));
	world_add_accel_node(world, &root);
	while (index < world->primitives_size / RT_PRIMITIVE_ALIGN)
	{
		primitive = get_primitive(world, index);
		vector_push_back(&all_indices, &index);
		index += world_primitive_size(primitive->shape_type) / RT_PRIMITIVE_ALIGN;
	}
	fprintf(stderr, "actual:%u vs vector%zu\n", world->primitives_count, vector_size(&all_indices));
	build_tree(
			0,
			world,
			&all_indices,
		accel_get_max_depth(world));
	vector_destroy(&all_indices, NULL);
}
