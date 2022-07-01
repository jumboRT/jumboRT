#include "world.h"
#include "world_impl.h"
#include "util.h"
#include "vector.h"
#include <math.h>


#include <stdio.h>

void build_tree(t_node_info *info);

t_vec get_vertex(const t_world *world, uint32_t index) {
	rt_assert(index < world->vertices_count, "out of bounds access in get_vertex");
	return (world->vertices[index].pos);
}

t_bounds get_bounds_triangle(const t_world *world, const t_shape_triangle *triangle) {
	t_vec	points[3];

	points[0] = get_vertex(world, triangle->a);
	points[1] = get_vertex(world, triangle->b);
	points[2] = get_vertex(world, triangle->c);
	return (bounds(
				vec_min(vec_min(points[0], points[1]), points[2]),
				vec_max(vec_max(points[0], points[1]), points[2])));
}

t_bounds get_bounds_sphere(const t_world *world, const t_shape_sphere *sphere) {
	(void) world;
	return (bounds(
				vec_sub(sphere->pos, vec(sphere->radius, sphere->radius, sphere->radius)),
				vec_add(sphere->pos, vec(sphere->radius, sphere->radius, sphere->radius))));
}

t_bounds get_bounds(const t_world *world, const t_primitive *primitive) {
	if (primitive->shape_type == RT_SHAPE_TRIANGLE)
		return (get_bounds_triangle(world, (const t_shape_triangle *) primitive));
	return (get_bounds_sphere(world, (const t_shape_sphere *) primitive));
}

const t_primitive *get_primitive(const t_world *world, uint32_t offset) {
	rt_assert(offset * RT_PRIMITIVE_ALIGN < world->primitives_size, "out of bounds access in get_primitive");
	return (
			(const t_primitive *)
			((const char *) world->primitives +
			(offset * RT_PRIMITIVE_ALIGN)));
}

t_bounds get_total_bounds(const t_world *world, t_vector *indices) {
	const t_primitive	*primitive;
	size_t				index;
	t_bounds			total_bounds;

	rt_assert(vector_size(indices) > 0, "empty indices");
	index = 1;
	total_bounds = get_bounds(world, get_primitive(world, *(uint32_t *) vector_at(indices, 0)));
	while (index < vector_size(indices)) {
		primitive = get_primitive(world, *(uint32_t *) vector_at(indices, index));
		total_bounds = bounds_max(total_bounds, get_bounds(world, primitive));
		++index;
	}
	return (total_bounds);
}

void interior_node_init(t_node_info *info, const t_node_info *child_info, const t_split *split) {
	t_accel_node	*node;

	node = info->world->accel_nodes + info->offset;
	node->a.split = split->offset;
	node->b.flags = split->axis;
	node->b.above_child |= (child_info->offset << 2);
}

uint32_t push_back_primitives(t_world *world, t_vector *indices) {
	uint32_t	first_index;
	uint64_t	index;

	rt_assert(vector_size(indices) >= 1, "empty indices vector");
	first_index = world_add_accel_index(world, vector_at(indices, 0));
	index = 1;
	while (index < vector_size(indices)) {
		world_add_accel_index(world, vector_at(indices, index));
		++index;
	}
	return (first_index);
}

void leaf_node_init(t_node_info *info) {
	uint32_t		primitive_count;
	t_accel_node	*node;

	node = info->world->accel_nodes + info->offset;
	primitive_count = vector_size(&info->indices);
	node->b.flags = 3;
	node->b.nprims |= (primitive_count << 2);
	if (primitive_count == 0)
		node->a.one_primitive = 0;
	else if (primitive_count == 1)
		node->a.one_primitive = *(uint32_t *) vector_at(&info->indices, 0);
	else
		node->a.one_primitive = push_back_primitives(info->world, &info->indices);
}

FLOAT calculate_cost(FLOAT total_sa, const FLOAT sub_sa[2], const uint32_t primitive_counts[2]) {
	FLOAT	inv_total_sa;
	FLOAT	below_chance;
	FLOAT	above_chance;
	FLOAT	cost;

	inv_total_sa = 1.0 / total_sa;
	below_chance = sub_sa[ACCEL_BELOW] * inv_total_sa;
	above_chance = sub_sa[ACCEL_ABOVE] * inv_total_sa;
	cost = (RT_TRAVERSAL_COST +
			RT_INTERSECT_COST *
			(1.0 - (RT_EMPTY_BONUS * (primitive_counts[ACCEL_BELOW] == 0 || primitive_counts[ACCEL_ABOVE] == 0))) *
			(below_chance * primitive_counts[ACCEL_BELOW] +
			 above_chance * primitive_counts[ACCEL_ABOVE]));
	return (cost);
}

void get_surface_areas(const t_bounds bounds, FLOAT offset, int axis, FLOAT out[2]) {
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

FLOAT get_split_cost(const t_bounds bounds, const t_split *split, int axis) {
	FLOAT		surface_areas[2];
	uint32_t	primitive_counts[2];

	get_surface_areas(bounds, split->offset, axis, surface_areas);
	primitive_counts[ACCEL_ABOVE] = split->prim_count[ACCEL_ABOVE];
	primitive_counts[ACCEL_BELOW] = split->prim_count[ACCEL_BELOW];
	return (calculate_cost(bounds_surf(bounds), surface_areas, primitive_counts));
}

int32_t edge_within_bounds(const t_edge *edge, const t_bounds bounds, int axis) {
	return (edge->offset > xyz(bounds.min, axis)
		&& edge->offset < xyz(bounds.max, axis));
}

int split_cmp(t_split a, t_split b) {
	return ((a.cost > b.cost) - (a.cost < b.cost));
}

void split_init(t_split *split, int axis, size_t edge_count) {
	split->axis = axis;
	split->cost = RT_HUGE_VAL;
	split->offset = RT_HUGE_VAL;
	split->prim_count[ACCEL_BELOW] = 0;
	split->prim_count[ACCEL_ABOVE] = edge_count / 2;
}

t_split get_best_split(t_bounds bounds, t_vector edges, int axis) {
	const t_edge	*edge;
	t_split			current;
	t_split			best;
	size_t			index;

	index = 0;
	split_init(&current, axis, vector_size(&edges));
	best.cost = RT_HUGE_VAL;
	while (index < vector_size(&edges)) {
		edge = vector_at(&edges, index);
		if (edge->type == EDGE_END)
			current.prim_count[ACCEL_ABOVE] -= 1;
		if (edge_within_bounds(edge, bounds, axis))
		{
			current.offset = edge->offset;
			current.cost = get_split_cost(bounds, &current, axis);
			if (split_cmp(current, best) < 0)
				best = current;
		}
		if (edge->type == EDGE_START)
			current.prim_count[ACCEL_BELOW] += 1;
		index++;
	}
	return (best);
}

void get_best_split_job(void *ctx_ptr, void *results_ptr, size_t id) {
	t_split		*results;
	t_node_info	*info;

	rt_assert(id < 3, "invalid id in get_best_split_job");
	info = ctx_ptr;
	results = results_ptr;
	results[id] = get_best_split(info->bounds, info->edges[id], id);
}

int32_t find_best_split(t_node_info *info, t_split *out) {
	t_jobs		jobs;
	t_jobs_item	items[3];
	t_split		results[3];

	jobs.start = get_best_split_job;
	jobs.ctx = info;
	jobs.items = items;
	jobs.count = 3;
	jobs.results = results;
	pool_run(info->pool, &jobs);

	if (split_cmp(results[1], results[0]) < 0)
		results[0] = results[1];
	if (split_cmp(results[2], results[0]) < 0)
		results[0] = results[2];
	*out = results[0];
	return (out->cost < RT_HUGE_VAL);
}

uint32_t new_node(t_world *world) {
	t_accel_node	node;
	return (world_add_accel_node(world, &node));
}

int32_t get_axis_side(
			const t_world		*world,
			const t_split		*split,
			const t_primitive	*primitive)
{
	t_bounds	primitive_bounds;
	FLOAT		max_offset;
	FLOAT		min_offset;
	FLOAT		axis_offset;

	primitive_bounds = get_bounds(world, primitive);
	max_offset = xyz(primitive_bounds.max, split->axis);
	min_offset = xyz(primitive_bounds.min, split->axis);
	axis_offset = split->offset;
	if (max_offset >= axis_offset && min_offset >= axis_offset)
		return (1); /* probably above, might be the wrong way around with below */
	else if (max_offset <= axis_offset && min_offset <= axis_offset)
		return (-1);
	return (0); /* object is in the axis */
}

/* filter edges must be a pointer to two vectors of edges */
void filter_edges(const t_world *world, const t_split *split, t_vector *org_edges, t_vector *new_edges, int32_t which) {
	size_t			index;
	int32_t			side;;
	const t_edge	*edge;

	rt_assert(which == ACCEL_ABOVE || which == ACCEL_BELOW, "invalid which in filter_edges");
	index = 0;
	vector_init(new_edges, sizeof(*edge));
	while (index < vector_size(org_edges)) {
		edge = vector_at(org_edges, index);
		side = get_axis_side(world, split, get_primitive(world, edge->index));
		if (which == ACCEL_BELOW) {
			if (side <= 0) {
				/*fprintf(stderr, "which:%d, split offset:%f axis:%d, min offset:%f max offset:%f index:%d\n", which, split->offset, split->axis, xyz(prim_bounds.min, split->axis), xyz(prim_bounds.max, split->axis), edge->index);*/
				vector_push_back(new_edges, edge);
			}
		} else {
			if (side >= 0) {
				/*fprintf(stderr, "which:%d, split offset:%f axis:%d, min offset:%f max offset:%f index:%d\n", which, split->offset, split->axis, xyz(prim_bounds.min, split->axis), xyz(prim_bounds.max, split->axis), edge->index);*/
				vector_push_back(new_edges, edge);
			}
		}
		++index;
	}
}

void create_interior_node_above(t_node_info *info, t_vector indices, const t_split *split) {
	t_bounds	child_bounds;
	t_node_info	child_info;
	uint32_t	offset;

	offset = new_node(info->world);
	child_bounds = bounds(vec_set(info->bounds.min, split->axis, split->offset), info->bounds.max);
	child_info.world = info->world;
	child_info.offset = offset;
	child_info.indices = indices;
	child_info.bounds = child_bounds;
	child_info.depth = info->depth - 1;
	child_info.pool = info->pool;
	filter_edges(info->world, split, &info->edges[0], &child_info.edges[0], ACCEL_ABOVE);
	filter_edges(info->world, split, &info->edges[1], &child_info.edges[1], ACCEL_ABOVE);
	filter_edges(info->world, split, &info->edges[2], &child_info.edges[2], ACCEL_ABOVE);
	interior_node_init(info, &child_info, split);
	build_tree(&child_info);
	vector_destroy(&child_info.edges[0], NULL);
	vector_destroy(&child_info.edges[1], NULL);
	vector_destroy(&child_info.edges[2], NULL);
}

void create_interior_node_below(t_node_info *info, t_vector indices, const t_split *split) {
	t_bounds	child_bounds;
	t_node_info	child_info;
	uint32_t	offset;

	offset = new_node(info->world);
	child_bounds = bounds(info->bounds.min, vec_set(info->bounds.max, split->axis, split->offset));
	child_info.world = info->world;
	child_info.offset = offset;
	child_info.indices = indices;
	child_info.bounds = child_bounds;
	child_info.depth = info->depth - 1;
	child_info.pool = info->pool;
	filter_edges(info->world, split, &info->edges[0], &child_info.edges[0], ACCEL_BELOW);
	filter_edges(info->world, split, &info->edges[1], &child_info.edges[1], ACCEL_BELOW);
	filter_edges(info->world, split, &info->edges[2], &child_info.edges[2], ACCEL_BELOW);
	build_tree(&child_info);
	vector_destroy(&child_info.edges[0], NULL);
	vector_destroy(&child_info.edges[1], NULL);
	vector_destroy(&child_info.edges[2], NULL);
}

void
	split_indices(t_node_info *info, const t_split *split, t_vector sub_indices[2]) {
	uint64_t	index;
	int32_t		side;

	index = 0;
	vector_init(&sub_indices[ACCEL_BELOW], sizeof(uint32_t));
	vector_init(&sub_indices[ACCEL_ABOVE], sizeof(uint32_t));
	while (index < vector_size(&info->indices))
	{
		side = get_axis_side(info->world, split, get_primitive(info->world, *(uint32_t *) vector_at(&info->indices, index)));

		if (side <= 0)
		{
			vector_push_back(&sub_indices[ACCEL_BELOW], vector_at(&info->indices, index));
		}
		if (side >= 0)
		{
			vector_push_back(&sub_indices[ACCEL_ABOVE], vector_at(&info->indices, index));
		}
		++index;
	}
}

void create_and_add_interior_nodes(t_node_info *info, const t_split *split) {
	t_vector	child_indices[2];

	split_indices(info, split, child_indices);
	create_interior_node_below(info, child_indices[ACCEL_BELOW], split);
	create_interior_node_above(info, child_indices[ACCEL_ABOVE], split);
	vector_destroy(&child_indices[ACCEL_BELOW], NULL);
	vector_destroy(&child_indices[ACCEL_ABOVE], NULL);
}

int32_t try_create_and_add_interior_nodes(t_node_info *info) {
	t_split	split;

	if (!find_best_split(info, &split))
		return (0);
	create_and_add_interior_nodes(info, &split);
	return (1);
}

void build_tree(t_node_info *info) {
	rt_assert(info->offset != 1627, "breakpoint");
	/*fprintf(stderr, "x count: %zu, y count: %zu, z count: %zu\n", vector_size(&info->edges[0]), vector_size(&info->edges[1]), vector_size(&info->edges[2]));*/
	if (info->depth == 0 || vector_size(&info->indices) <= RT_MAX_PRIMITIVES)
	{
		leaf_node_init(info);
		return;
	}
	if (!try_create_and_add_interior_nodes(info))
	{
		leaf_node_init(info);
		return;
	}
}

t_vector get_indices(const t_world *const world) {
	const t_primitive	*primitive;
	t_vector			indices;
	size_t				index;

	index = 0;
	vector_init(&indices, sizeof(uint32_t));
	while (index < world->primitives_size / RT_PRIMITIVE_ALIGN) {
		primitive = get_primitive(world, index);
		vector_push_back(&indices, &index);
		index += world_primitive_size(primitive->shape_type) / RT_PRIMITIVE_ALIGN;
	}
	return (indices);
}

void add_edges(const t_world *world, t_vector *indices, t_vector edges[3]) {
	size_t		index;
	int			axis;
	t_bounds	prim_bounds;
	t_edge		edge;

	index = 0;
	while (index < vector_size(indices)) {
		axis = 0;
		while (axis < 3) {
			prim_bounds = get_bounds(world, get_primitive(world, *(uint32_t *) vector_at(indices, index)));
			edge.index = *(uint32_t *) vector_at(indices, index);
			edge.offset = xyz(prim_bounds.min, axis);
			edge.type = EDGE_START;
			vector_push_back(&edges[axis], &edge);
			edge.offset = xyz(prim_bounds.max, axis);
			edge.type = EDGE_END;
			vector_push_back(&edges[axis], &edge);
			++axis;
		}
		++index;
	}
}

int32_t
	cmp_edge(const void *edge0_ptr, const void *edge1_ptr, void *ctx_ptr)
{
	const t_edge	*edge0; 
	const t_edge	*edge1; 

	(void) ctx_ptr;
	edge0 = edge0_ptr;
	edge1 = edge1_ptr;
	if (edge0->offset != edge1->offset)
		return ((edge0->offset > edge1->offset) - (edge0->offset < edge1->offset));
	else if (edge0->type != edge1->type)
		return ((edge0->type > edge1->type) - (edge0->type < edge1->type));
	else
		return ((edge0->index > edge1->index) - (edge0->index < edge1->index));
}

void get_edges(const t_world *world, t_vector *indices, t_vector edges[3]) {
	vector_init(&edges[0], sizeof(t_edge));
	vector_init(&edges[1], sizeof(t_edge));
	vector_init(&edges[2], sizeof(t_edge));
	add_edges(world, indices, edges);
	vector_sort(&edges[0], cmp_edge, NULL);
	vector_sort(&edges[1], cmp_edge, NULL);
	vector_sort(&edges[2], cmp_edge, NULL);
}

uint32_t accel_max_depth(const t_world *world) {
	return (8.0 + 1.3 * log2(world->primitives_count));
	return (16);
}

/* This function is only meant to be called for the root node */
void create_node_info(t_node_info *info, t_world *world) {
	info->world = world;
	info->offset = 0;
	info->indices = get_indices(world);
	info->bounds = get_total_bounds(world, &info->indices);
	info->depth = accel_max_depth(world);
	get_edges(world, &info->indices, info->edges);
	info->pool = rt_malloc(sizeof(*info->pool));
	pool_create(info->pool, 0);
}

void destroy_node_info(t_node_info *info) {
	vector_destroy(&info->indices, NULL);
	vector_destroy(&info->edges[0], NULL);
	vector_destroy(&info->edges[1], NULL);
	vector_destroy(&info->edges[2], NULL);
	pool_destroy(info->pool);
	rt_free(info->pool);
}

void world_accel(t_world *world) {
	t_node_info		info;
	t_accel_node	root;

	world_add_accel_node(world, &root);
	create_node_info(&info, world);
	build_tree(&info);
	destroy_node_info(&info);
}
