#include "accel.h"
#include "accel_impl.h"

#include "util.h"
#include "world.h"
#include "world_impl.h"
#include "shape.h"

static int
	world_info_edge_cmp(const void *a_ptr, const void *b_ptr)
{
	const t_edge *const	a = a_ptr;
	const t_edge *const	b = b_ptr;

	return ((a->offset > b->offset) - (a->offset < b->offset));
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

void
	world_info_init_free(t_tree_info *tree, t_node_info *node)
{
	uint32_t	new_depth;
	size_t		offset;

	new_depth = world_max_depth(tree->prims_count);
	offset = 0;
	while (node->depth - offset > new_depth)
	{
		rt_free(tree->edges[node->depth - offset].edges[AXIS_X]);
		rt_free(tree->edges[node->depth - offset].edges[AXIS_Y]);
		rt_free(tree->edges[node->depth - offset].edges[AXIS_Z]);
		offset += 1;
	}
	node->depth = world_max_depth(tree->prims_count);
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
		primitive = (t_primitive *)((char *) world->primitives + offset);
		if (prim_is_degenerate(primitive))
			world_add_accel_degenerate(world, offset / RT_PRIMITIVE_ALIGN);
		else
		{
			prim.index = offset / RT_PRIMITIVE_ALIGN;
			prim.bounds = prim_bounds(primitive, world);
			world_info_add_prim(tree, node, prim);
		}
		offset += world_primitive_size(prim_type(primitive));
	}
	world_info_init_free(tree, node);
	view_sort_array(tree->edges[0].edges[AXIS_X], tree->edges[0].count,
		sizeof(t_edge), world_info_edge_cmp);
	view_sort_array(tree->edges[0].edges[AXIS_Y], tree->edges[0].count,
		sizeof(t_edge), world_info_edge_cmp);
	view_sort_array(tree->edges[0].edges[AXIS_Z], tree->edges[0].count,
		sizeof(t_edge), world_info_edge_cmp);
}
