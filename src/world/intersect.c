/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   intersect.c                                    #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:27 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:27 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "world.h"

#include "shape.h"
#include "accel.h"

static void
	world_intersect_primitives(const GLOBAL t_world *world,
			t_ray ray, t_world_hit *hit)
{
	t_uint64					index;
	const GLOBAL char			*primitives;
	const GLOBAL t_primitive	*primitive;
	t_world_hit					current;
	t_ray_min					ray_min;

	index = 0;
	primitives = (const GLOBAL char *) world->primitives;
	ray_min.ray = ray;
	ray_min.min = RT_TINY_VAL;
	while (index < world->primitives_size)
	{
		primitive = (const GLOBAL t_primitive *)(primitives + index);
		if (prim_intersect(primitive, world, ray_min, &current)
			&& current.hit.t < hit->hit.t)
			*hit = current;
		index += world_primitive_size(prim_type(primitive));
	}
}

static void
	world_intersect_degenerates(const GLOBAL t_world *world,
			t_ray ray, t_world_hit *hit)
{
	t_uint32					index;
	const GLOBAL t_primitive	*primitive;
	t_world_hit					current;
	t_ray_min					ray_min;

	index = 0;
	ray_min.ray = ray;
	ray_min.min = RT_TINY_VAL;
	while (index < world->accel_degenerates_count)
	{
		primitive = get_prim_const(world, world->accel_degenerates[index]);
		if (prim_intersect(primitive, world, ray_min, &current)
			&& current.hit.t < hit->hit.t)
			*hit = current;
		index += 1;
	}
}

void
	world_intersect_step(const GLOBAL t_world *world, t_ray ray,
			struct s_intersect_ctx *ctx, t_world_hit *hit)
{
	const GLOBAL t_primitive	*prim;
	t_world_hit					current;
	t_ray_min					ray_min;

	if (ctx->prim_index < ctx->prim_count)
	{
		prim = get_prim_const(world, ctx->prims[ctx->prim_index]);
		ray_min.ray = ray;
		ray_min.min = ctx->min_t;
		if (prim_intersect(prim, world, ray_min, &current)
			&& current.hit.t < hit->hit.t)
			*hit = current;
		ctx->prim_index += 1;
	}
}

void
	world_intersect(const GLOBAL t_world *world, t_ray ray, t_world_hit *hit)
{
	if (ACCEL_USE_TREE)
	{
		world_intersect_degenerates(world, ray, hit);
		world_intersect_tree(world, ray, hit);
	}
	else
		world_intersect_primitives(world, ray, hit);
}
