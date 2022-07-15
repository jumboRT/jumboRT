#include "parser.h"

#include "world_impl.h"
#include "util.h"
#include "util.h"
#include <libft.h>

static int
	has_prefix(const t_parse_ctx *ctx, const char *prefix)
{
	const char *word;

	word = ctx->data;
	while (ft_isspace(*word))
		++word;
	return (ft_strncmp(word, prefix, ft_strlen(prefix)) == 0);
}

void
	rt_material(t_parse_ctx *ctx, t_world *world, t_primitive *shape)
{
	char		*keyword;
	int64_t		mat_index;
	t_material	material;

	if (has_prefix(ctx, "mat_"))
	{
		keyword = rt_keyword(ctx, "mat_");
		mat_index = world_get_material(world, rt_hash(keyword));
		if (mat_index < 0)
			rt_parse_error(ctx, "undefined material %s", keyword);
		shape->data |= (uint32_t) mat_index << 8;
		rt_free(keyword);
	}
	else
	{
		material.emission = vec(0, 0, 0);
		material.albedo = rt_color(ctx);
		material.reflective = 0;
		shape->data |= world_add_material(world, &material, sizeof(material)) << 8;
	}
}

void
	rt_exec_material(t_world *world, t_parse_ctx *ctx)
{
	t_material		material;
	char			*keyword;

	keyword = rt_keyword(ctx, "mat_");
	material.id = rt_hash(keyword);
	material.emission = rt_color(ctx);
	material.albedo = rt_color(ctx);
	material.refractive_index = rt_float(ctx);
	material.reflective = rt_bool(ctx);
	material.density = rt_float(ctx);
	world_add_material(world, &material, sizeof(material));
	rt_free(keyword);
}
