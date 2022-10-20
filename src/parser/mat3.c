#include "parser.h"
#include "mat.h"
#include "world_impl.h"
#include "bsdf.h"

void
	rt_exec_normal(t_world *world, t_parse_ctx *ctx)
{
	(void) world;
	rt_check_in_mat(ctx);
	ctx->mat->normal_map = rt_texture(world, ctx);
	ctx->mat->flags |= RT_MAT_HAS_NORMAL;
}

void
	rt_exec_bump(t_world *world, t_parse_ctx *ctx)
{
	(void) world;
	rt_check_in_mat(ctx);
	ctx->mat->bump_map = rt_texture(world, ctx);
	ctx->mat->flags |= RT_MAT_HAS_BUMP;
}

void
	rt_exec_volume(t_world *world, t_parse_ctx *ctx)
{
	(void) world;
	rt_check_in_mat(ctx);
	ctx->mat->density = rt_float(ctx);
	ctx->mat->flags |= RT_MAT_HAS_VOLUME;
	ctx->mat->volume.begin = world->bxdfs_count;
	ctx->mat->volume.end = world->bxdfs_count;
}

void
	rt_exec_refractive_index(t_world *world, t_parse_ctx *ctx)
{
	(void) world;
	rt_check_in_mat(ctx);
	ctx->mat->refractive_index = rt_float(ctx);
}
