/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   trace_volume.c                                 #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:27 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:27 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "world.h"
#include "mat.h"

static void
	enable_volume(t_trace_ctx *ctx, const GLOBAL t_material *mat)
{
	ctx->volumes[ctx->volume_size] = mat;
	ctx->volume_size += 1;
}

static void
	disable_volume(t_trace_ctx *ctx, const GLOBAL t_material *mat)
{
	t_uint32	vi;

	vi = 0;
	while (vi < ctx->volume_size)
	{
		if (ctx->volumes[vi] == mat)
		{
			ctx->volume_size -= 1;
			ctx->volumes[vi] = ctx->volumes[ctx->volume_size];
			break ;
		}
		vi += 1;
	}
}

void
	toggle_volume(t_trace_ctx *ctx, const t_world_hit *hit, t_vec new_dir)
{
	float		id;
	float		od;

	if (hit->is_volume || !(hit->mat->flags & RT_MAT_HAS_VOLUME))
		return ;
	id = vec_dot(hit->hit.geometric_normal, ctx->ray.dir);
	od = vec_dot(hit->hit.geometric_normal, new_dir);
	if (id < 0 && od < 0 && ctx->volume_size < RT_MAX_VOLUMES)
		enable_volume(ctx, hit->mat);
	else if (id > 0 && od > 0)
		disable_volume(ctx, hit->mat);
}

void
	intersect_volume(const t_trace_ctx *ctx, t_world_hit *hit)
{
	t_uint32	vi;
	float		t;

	vi = 0;
	while (vi < ctx->volume_size)
	{
		t = rt_log(rt_random_float(&ctx->ctx->seed))
			/ -ctx->volumes[vi]->density;
		if (t < hit->hit.t)
		{
			hit->hit.t = t;
			hit->prim = 0;
			hit->mat = ctx->volumes[vi];
			hit->is_volume = 1;
		}
		vi += 1;
	}
}
