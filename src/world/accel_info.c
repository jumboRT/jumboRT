#include "world_impl.h"

#include "util.h"

void
	world_info_create(t_tree_info *tree, t_node_info *node, t_world *world)
{
	size_t		i;
	t_vector	*edges;

	node->tree = tree;
	node->offset = 0;
	node->depth = world_max_depth(world);
	node->bounds = world_bounds(world);
	tree->world = world;
	tree->edges = rt_malloc(sizeof(*tree->edges) * (node->depth + 1));
	node->edges = tree->edges;
	vector_create(&tree->prims, sizeof(t_prim_info), world->primitives_count);
	i = 0;
	while (i < node->depth + 1)
	{
		edges = tree->edges[i].edges;
		vector_create(&edges[AXIS_X], sizeof(t_edge), world->primitives_count * 2);
		vector_create(&edges[AXIS_Y], sizeof(t_edge), world->primitives_count * 2);
		vector_create(&edges[AXIS_Z], sizeof(t_edge), world->primitives_count * 2);
		i += 1;
	}
}

void
	world_info_add_prim(t_tree_info *tree, t_prim_info prim, uint32_t index)
{
	t_edge	edge;

	edge.index = index;
	edge.type = EDGE_START;
	edge.offset = x(prim.bounds.min);
	vector_push(&tree->edges[0].edges[AXIS_X], &edge);
	edge.offset = y(prim.bounds.min);
	vector_push(&tree->edges[0].edges[AXIS_Y], &edge);
	edge.offset = z(prim.bounds.min);
	vector_push(&tree->edges[0].edges[AXIS_Z], &edge);
	edge.type = EDGE_END;
	edge.offset = x(prim.bounds.max);
	vector_push(&tree->edges[0].edges[AXIS_X], &edge);
	edge.offset = y(prim.bounds.max);
	vector_push(&tree->edges[0].edges[AXIS_Y], &edge);
	edge.offset = z(prim.bounds.max);
	vector_push(&tree->edges[0].edges[AXIS_Z], &edge);
	vector_push(&tree->prims, &prim);
}

int
	world_info_edge_cmp(const void *a_ptr, const void *b_ptr)
{
	const t_edge	*const a = a_ptr;
	const t_edge	*const b = b_ptr;

	if (a->offset != b->offset)
		return ((a->offset > b->offset) - (a->offset < b->offset));
	else if (a->type != b->type)
		return ((a->type > b->type) - (a->type < b->type));
	else
		return ((a->index > b->index) - (a->index < b->index));
}

void
	world_info_init(t_tree_info *tree, t_world *world)
{
	size_t		offset;
	uint32_t	index;
	t_primitive	*primitive;
	t_prim_info	prim;

	offset = 0;
	index = 0;
	while (offset < world->primitives_size)
	{
		primitive = (t_primitive *) ((char *) world->primitives + offset);
		prim.index = offset / RT_PRIMITIVE_ALIGN;
		prim.bounds = get_bounds(world, primitive);
		world_info_add_prim(tree, prim, index);
		offset += world_primitive_size(primitive->shape_type);
		index += 1;
	}
	view_sort(tree->edges[0].edges[AXIS_X].view, world_info_edge_cmp);
	view_sort(tree->edges[0].edges[AXIS_Y].view, world_info_edge_cmp);
	view_sort(tree->edges[0].edges[AXIS_Z].view, world_info_edge_cmp);
}

void
	world_info_destroy(t_tree_info *tree, t_node_info *node)
{
	size_t		i;
	t_vector	*edges;

	i = 0;
	while (i < node->depth + 1)
	{
		edges = tree->edges[i].edges;
		vector_destroy(&edges[AXIS_X], NULL);
		vector_destroy(&edges[AXIS_Y], NULL);
		vector_destroy(&edges[AXIS_Z], NULL);
		i += 1;
	}
	vector_destroy(&tree->prims, NULL);
	rt_free(tree->edges);
}

