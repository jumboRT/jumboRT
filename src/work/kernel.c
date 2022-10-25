/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   kernel.c                                       #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:10 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:10 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "world.h"

#ifdef RT_OPENCL

/* it is literally impossible to make this conform to the norm */
/* so we use this trick with comments (shh don't tell anyone) */

//\\
/*

__kernel void
	work_kernel(
		GLOBAL t_world *world,
		GLOBAL t_context *ctx,
		t_uint64 begin,
		t_uint64 end,
		GLOBAL t_result *results,
		GLOBAL unsigned int *index,
		GLOBAL void *primitives,
		GLOBAL void *materials,
		GLOBAL void *vertices,
		GLOBAL void *accel_nodes,
		GLOBAL void *accel_indices,
		GLOBAL void *accel_degenerates,
		GLOBAL void *texture_data,
		GLOBAL void *textures,
		GLOBAL void *bxdfs,
		GLOBAL void *lights,
		GLOBAL void *leaf_data)
{
	GLOBAL t_context	*my_ctx;
	t_trace_all_ctx		ta_ctx;

	world->primitives = primitives;
	world->materials = materials;
	world->vertices = vertices;
	world->accel_nodes = accel_nodes;
	world->accel_indices = accel_indices;
	world->accel_degenerates = accel_degenerates;
	world->texture_data = texture_data;
	world->textures = textures;
	world->bxdfs = bxdfs;
	world->lights = lights;
	world->leaf_data = leaf_data;
	my_ctx = &ctx[get_global_id(0)];
	ta_ctx.begin = begin;
	ta_ctx.end = end;
	ta_ctx.results = results;
	ta_ctx.index = index;
	world_trace_all(world, my_ctx, &ta_ctx);
}

/**/

#endif
