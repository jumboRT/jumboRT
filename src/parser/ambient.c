#include "parser.h"

#include "world_impl.h"
#include "util.h"

void
	rt_exec_ambient(t_world *world, t_parse_ctx *ctx)
{
	char		*keyword;
	t_material	material;

	if (has_prefix(ctx, "mat_"))
	{
		keyword = rt_keyword(ctx, "mat_");
		world->ambient_mat = mat_by_name(world, ctx, keyword);
		world->flags |= RT_WORLD_HAS_AMBIENT;
		rt_free(keyword);
	}
	else
	{
		material_init(&material, world);
		material.brightness = rt_float_range(ctx, 0.0, 1.0);
		material.emission = rt_filter(world, ctx);
		material.flags |= RT_MAT_EMITTER;
		world->ambient_mat = world_add_material(world, &material);
		world->flags |= RT_WORLD_HAS_AMBIENT;
	}
}

