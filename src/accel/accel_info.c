/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   accel_info.c                                   #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:26 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:26 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "accel.h"
#include "accel_impl.h"
#include "aabb.h"
#include "shape.h"
#include "world.h"
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
