/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   accel_algo.c                                   #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:26 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:26 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "accel.h"
#include "accel_impl.h"
#include "hash.h"

#include "world.h"
#include "world_impl.h"
#include "util.h"
#include "ser.h"
#include "perf.h"

#include <ft_printf.h>

static void
	world_split_edges(t_node_info *node, t_split *split,
			t_uint8 axis, size_t *counts)
{
	t_edge	*below_above_cur_end[4];
	int		side;

	below_above_cur_end[0] = node->edges[0].edges[axis];
	below_above_cur_end[1] = node->edges[1].edges[axis];
	below_above_cur_end[2] = below_above_cur_end[0];
	below_above_cur_end[3] = below_above_cur_end[0] + node->edges[0].count;
	while (below_above_cur_end[2] != below_above_cur_end[3])
	{
		side = world_axis_side(node->tree, split,
				below_above_cur_end[2]->index);
		if (side <= 0)
		{
			*below_above_cur_end[1] = *below_above_cur_end[2];
			below_above_cur_end[1] += 1;
		}
		if (side >= 0)
		{
			*below_above_cur_end[0] = *below_above_cur_end[2];
			below_above_cur_end[0] += 1;
		}
		below_above_cur_end[2] += 1;
	}
	counts[0] = below_above_cur_end[0] - node->edges[0].edges[axis];
	counts[1] = below_above_cur_end[1] - node->edges[1].edges[axis];
}

static void	world_plant_tree(t_node_info *node);

static void
	world_plant_tree_inner(t_node_info *node, t_split *split,
			t_node_info *child, size_t counts[2])
{
	world_split_edges(node, split, AXIS_X, counts);
	world_split_edges(node, split, AXIS_Y, counts);
	world_split_edges(node, split, AXIS_Z, counts);
	node->edges[0].count = counts[0];
	node->edges[1].count = counts[1];
	child->tree = node->tree;
	child->offset = new_node(node->tree->world);
	child->depth = node->depth - 1;
	child->edges = node->edges + 1;
	child->bounds = bounds(node->bounds.min,
			vec_set(node->bounds.max, split->axis, split->offset));
	world_plant_tree(child);
	child->offset = new_node(node->tree->world);
	child->edges = node->edges;
	child->bounds = bounds(vec_set(node->bounds.min,
				split->axis, split->offset), node->bounds.max);
	interior_node_init(node, child, split);
	world_plant_tree(child);
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
			world_plant_tree_inner(node, &split, &child, counts);
			return ;
		}
	}
	leaf_node_init(node);
}

void
	world_accel(t_world *world)
{
	t_accel_init_ctx	ctx;
	t_uint64			hash;
	char				file[64];
	t_perf				perf;

	perf_start(&perf);
	hash = hash_world(world, RT_HASH_GEOMETRY);
	perf_split(&perf, "compute hash");
	ft_sprintf(file, ".%08x.jrtastic", (unsigned int) hash);
	ft_printf("world hash: %08x%08x\n",
		(unsigned int)(hash >> 32 & 0xFFFFFFFFU),
		(unsigned int)(hash & 0xFFFFFFFFU));
	if (!world_accel_load(world, file, hash))
	{
		world_add_accel_node(world, &ctx.root);
		world_info_create(&ctx.tree, &ctx.node, world);
		world_info_init(&ctx.tree, &ctx.node, world);
		world_plant_tree(&ctx.node);
		world_attach_ropes(world, ctx.node.bounds);
		world_info_destroy(&ctx.tree, &ctx.node);
		world_accel_save(world, file, hash);
	}
}
