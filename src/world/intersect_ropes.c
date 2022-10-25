/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   intersect_ropes.c                              #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:33 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:33 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "world.h"

#if ACCEL_USE_ROPES

static void
	world_intersect_find_leaf(const GLOBAL t_world *world, t_ray ray,
			struct s_intersect_ctx *ctx)
{
	float	org_t;
	float	dir_t;
	float	split_t;

	while (!is_leaf(*ctx->node))
	{
		org_t = xyz(ray.org, split_axis(*ctx->node));
		dir_t = xyz(ray.dir, split_axis(*ctx->node));
		split_t = split_pos(*ctx->node);
		if (org_t + dir_t * ctx->min_t < split_t)
			ctx->node = ctx->node + 1;
		else
			ctx->node = world->accel_nodes + above_child(*ctx->node);
	}
}

static void
	world_intersect_tree_exit(t_ray ray, const GLOBAL t_rope_data *rope,
			t_uint32 *exit_rope, float *exit_distance)
{
	float		org_t;
	float		dir_t;
	t_uint32	index;
	t_uint32	rope_index;
	float		distance;

	index = 0;
	while (index < 3)
	{
		org_t = xyz(ray.org, index);
		dir_t = xyz(ray.dir, index);
		rope_index = index + (dir_t > 0) * 3;
		distance = (rope->bounds[rope_index] - org_t) / dir_t;
		if (dir_t != 0 && distance < *exit_distance)
		{
			*exit_distance = distance;
			*exit_rope = rope->ropes[rope_index];
		}
		index += 1;
	}
}

static int
	world_intersect_tree_step(const GLOBAL t_world *world, t_ray ray,
			struct s_intersect_ctx *ctx, t_world_hit *hit)
{
	t_uint32					exit_rope;
	float						exit_distance;
	const GLOBAL t_leaf_data	*leaf;

	if (ctx->prim_index >= ctx->prim_count)
	{
		world_intersect_find_leaf(world, ray, ctx);
		ctx->prims = node_prims(world, ctx->node);
		ctx->prim_index = 0;
		ctx->prim_count = nprims(*ctx->node);
	}
	world_intersect_step(world, ray, ctx, hit);
	if (ctx->prim_index >= ctx->prim_count)
	{
		exit_rope = 0xFFFFFFFF;
		exit_distance = RT_HUGE_VAL;
		leaf = &world->leaf_data[ctx->node->leaf_data_index];
		world_intersect_tree_exit(ray, &leaf->rope_data,
			&exit_rope, &exit_distance);
		if (exit_rope == 0xFFFFFFFF)
			return (0);
		ctx->node = world->accel_nodes + exit_rope;
		ctx->min_t = rt_max(ctx->min_t, exit_distance - RT_TINY_VAL);
	}
	return (1);
}

void
	world_intersect_tree(const GLOBAL t_world *world, t_ray ray,
			t_world_hit *hit)
{
	struct s_intersect_ctx	ctx;

	ctx.min_t = RT_TINY_VAL;
	ctx.node = world->accel_nodes;
	ctx.prim_index = 0;
	ctx.prim_count = 0;
	while (ctx.min_t < hit->hit.t)
		if (!world_intersect_tree_step(world, ray, &ctx, hit))
			return ;
}

#endif
