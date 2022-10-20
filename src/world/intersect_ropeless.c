#include "world.h"
#include "accel.h"
#include "shape.h"

#if !ACCEL_USE_ROPES

static uint32_t
	world_intersect_find_next_child(const GLOBAL t_world *world,
			struct s_intersect_ctx *ctx, float a, float b)
{
	uint32_t	next_child;

	if (a < b)
	{
		next_child = above_child(*ctx->node);
		ctx->node = ctx->node + 1;
	}
	else
	{
		next_child = ctx->node - world->accel_nodes + 1;
		ctx->node = world->accel_nodes + above_child(*ctx->node);
	}
	return (next_child);
}

static void
	world_intersect_find_leaf(const GLOBAL t_world *world, t_ray ray,
			struct s_intersect_ctx *ctx)
{
	float		org_t;
	float		dir_t;
	float		split_t;
	float		plane_t;
	uint32_t	next_child;

	while (!is_leaf(*ctx->node))
	{
		org_t = xyz(ray.org, split_axis(*ctx->node));
		dir_t = xyz(ray.dir, split_axis(*ctx->node));
		split_t = split_pos(*ctx->node);
		next_child = world_intersect_find_next_child(world, ctx,
				org_t + dir_t * ctx->min_t, split_t);
		if (dir_t != 0)
		{
			plane_t = (split_t - org_t) / dir_t;
			if (plane_t >= ctx->min_t && plane_t < ctx->max_t)
			{
				ctx->stack[ctx->stack_index].index = next_child;
				ctx->stack[ctx->stack_index].max = ctx->max_t;
				ctx->stack_index += 1;
				ctx->max_t = plane_t;
			}
		}
	}
}

static int
	world_intersect_tree_step(const GLOBAL t_world *world, t_ray ray,
			struct s_intersect_ctx *ctx, t_world_hit *hit)
{
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
		if (ctx->stack_index == 0)
			return (0);
		ctx->min_t = rt_max(ctx->min_t, ctx->max_t - RT_TINY_VAL);
		ctx->stack_index -= 1;
		ctx->node = world->accel_nodes + ctx->stack[ctx->stack_index].index;
		ctx->max_t = ctx->stack[ctx->stack_index].max;
	}
	return (1);
}

void
	world_intersect_tree(const GLOBAL t_world *world,
			t_ray ray, t_world_hit *hit)
{
	struct s_intersect_ctx	ctx;

	ctx.min_t = RT_TINY_VAL;
	ctx.max_t = RT_HUGE_VAL;
	ctx.stack_index = 0;
	ctx.node = world->accel_nodes;
	ctx.prim_index = 0;
	ctx.prim_count = 0;
	while (ctx.min_t < hit->hit.t)
		if (!world_intersect_tree_step(world, ray, &ctx, hit))
			return ;
}

#endif
