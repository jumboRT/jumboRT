#include "parser.h"

#include "mat.h"
#include "util.h"
#include "shape.h"
#include "world_impl.h"
#include <libft.h>

uint32_t
	rt_material_int(t_parse_ctx *ctx, t_world *world)
{
	char		*keyword;
	uint32_t	mat_index;

	if (has_prefix(ctx, "mat_"))
	{
		keyword = rt_keyword(ctx, "mat_");
		mat_index = mat_by_name(world, ctx, keyword);
		rt_free(keyword);
		return (mat_index);
	}
	else
	{
		mat_index = mat_by_color(world, ctx, rt_color(ctx));
		return (mat_index);
	}
}

void
	rt_material(t_parse_ctx *ctx, t_world *world, t_primitive *shape)
{
	if (ctx->mat_use_set)
		shape->data |= ctx->mat_use << 8;
	else
		shape->data |= rt_material_int(ctx, world) << 8;
}

void
	rt_exec_mat_use(t_world *world, t_parse_ctx *ctx)
{
	ctx->mat_use_set = 1;
	ctx->mat_use = rt_material_int(ctx, world);
}

void
	rt_exec_mat_beg(t_world *world, t_parse_ctx *ctx)
{
	t_material		material;
	char			*keyword;
	uint32_t		mat_index;

	material_init(&material, world);
	keyword = rt_keyword(ctx, "mat_");
	mat_index = world_add_material(world, &material);
	ctx->mat = get_mat(world, mat_index);
	mat_add(ctx, keyword, mat_index);
	rt_free(keyword);
}

void
	rt_exec_mat_end(t_world *world, t_parse_ctx *ctx)
{
	(void) world;
	rt_check_in_mat(ctx);
	ctx->mat = NULL;
}
