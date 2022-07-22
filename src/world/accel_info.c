#include "world_impl.h"

#include "util.h"

void
	world_info_create(t_tree_info *tree, t_node_info *node, t_world *world)
{
	size_t		i;

	node->tree = tree;
	node->offset = 0;
	node->depth = world_max_depth(world->primitives_count);
	node->bounds = bounds_0();
	tree->world = world;
	tree->edges = rt_malloc(sizeof(*tree->edges) * (node->depth + 1));
	node->edges = tree->edges;
	tree->prims_count = 0;
	tree->prims = rt_malloc(sizeof(t_prim_info) * world->primitives_count);
	i = 0;
	while (i < node->depth + 1)
	{
		tree->edges[0].count = 0;
		tree->edges[i].edges[AXIS_X] = rt_malloc(sizeof(t_edge)
				* world->primitives_count * 2);
		tree->edges[i].edges[AXIS_Y] = rt_malloc(sizeof(t_edge)
				* world->primitives_count * 2);
		tree->edges[i].edges[AXIS_Z] = rt_malloc(sizeof(t_edge)
				* world->primitives_count * 2);
		i += 1;
	}
}

static void
	world_info_add_prim(t_tree_info *tree, t_node_info *node, t_prim_info prim)
{
	t_edge	edge;

	edge.index = tree->prims_count;
	edge.type = EDGE_START;
	tree->prims_count += 1;
	tree->edges[0].count += 2;
	edge.offset = x(prim.bounds.min);
	tree->edges[0].edges[AXIS_X][edge.index * 2 + 0] = edge;
	edge.offset = y(prim.bounds.min);
	tree->edges[0].edges[AXIS_Y][edge.index * 2 + 0] = edge;
	edge.offset = z(prim.bounds.min);
	tree->edges[0].edges[AXIS_Z][edge.index * 2 + 0] = edge;
	edge.type = EDGE_END;
	edge.offset = x(prim.bounds.max);
	tree->edges[0].edges[AXIS_X][edge.index * 2 + 1] = edge;
	edge.offset = y(prim.bounds.max);
	tree->edges[0].edges[AXIS_Y][edge.index * 2 + 1] = edge;
	edge.offset = z(prim.bounds.max);
	tree->edges[0].edges[AXIS_Z][edge.index * 2 + 1] = edge;
	tree->prims[edge.index] = prim;
	node->bounds = bounds_max(node->bounds, prim.bounds);
}

static int
	world_info_edge_cmp(const void *a_ptr, const void *b_ptr)
{
	const t_edge	*const a = a_ptr;
	const t_edge	*const b = b_ptr;

	return ((a->offset > b->offset) - (a->offset < b->offset));
}

void
	world_info_init(t_tree_info *tree, t_node_info *node, t_world *world)
{
	size_t		offset;
	t_primitive	*primitive;
	t_prim_info	prim;

	offset = 0;
	while (offset < world->primitives_size)
	{
		primitive = (t_primitive *) ((char *) world->primitives + offset);
		if (prim_is_infinite(primitive))
			world_add_accel_degenerate(world, offset / RT_PRIMITIVE_ALIGN);
		else
		{
			prim.index = offset / RT_PRIMITIVE_ALIGN;
			prim.bounds = prim_bounds(primitive, world);
			world_info_add_prim(tree, node, prim);
		}
		offset += world_primitive_size(prim_type(primitive));
	}
	node->depth = world_max_depth(tree->prims_count);
	view_sort_array(tree->edges[0].edges[AXIS_X], tree->edges[0].count,
			sizeof(t_edge), world_info_edge_cmp);
	view_sort_array(tree->edges[0].edges[AXIS_Y], tree->edges[0].count,
			sizeof(t_edge), world_info_edge_cmp);
	view_sort_array(tree->edges[0].edges[AXIS_Z], tree->edges[0].count,
			sizeof(t_edge), world_info_edge_cmp);
}

void
	world_info_destroy(t_tree_info *tree, t_node_info *node)
{
	size_t		i;

	i = 0;
	while (i < node->depth + 1)
	{
		rt_free(tree->edges[i].edges[AXIS_X]);
		rt_free(tree->edges[i].edges[AXIS_Y]);
		rt_free(tree->edges[i].edges[AXIS_Z]);
		i += 1;
	}
	rt_free(tree->prims);
	rt_free(tree->edges);
}

