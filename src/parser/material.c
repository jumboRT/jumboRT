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

	if (has_prefix(ctx, "mat_"))
	{
		keyword = rt_keyword(ctx, "mat_");
		mat_index = mat_by_name(world, ctx, keyword);
		shape->data |= (uint32_t) mat_index << 8;
		rt_free(keyword);
	}
	else
	{
		mat_index = mat_by_color(world, ctx, rt_color(ctx));
		shape->data |= (uint32_t) mat_index << 8;
	}
}

void
	rt_exec_mat_beg(t_world *world, t_parse_ctx *ctx)
{
	t_material		material;
	char			*keyword;
	uint32_t		mat_index;

	material_init(&material);
	keyword = rt_keyword(ctx, "mat_");
	material.id = rt_hash(keyword);
	mat_index = world_add_material(world, &material, sizeof(material));
	ctx->mat = get_mat(world, mat_index);
	mat_add(ctx, keyword, mat_index);
	rt_free(keyword);
}

void
	rt_exec_emission(t_world *world, t_parse_ctx *ctx)
{
	char	*keyword;

	if (ctx->mat == NULL)
	    rt_parse_error(ctx, "unexpected directive, did not start a material");
	if (has_prefix(ctx, "tex_"))
	{
		keyword = rt_keyword(ctx, "tex_");
		ctx->mat->has_texture |= RT_TEX_EMISSION_BIT;
		ctx->mat->tex_emission_offset = tex_by_name(world, ctx, keyword);
		rt_free(keyword);
	}
	else
	{
		ctx->mat->emission = rt_color(ctx);
	}
}

void
	rt_exec_albedo(t_world *world, t_parse_ctx *ctx)
{
	char	*keyword;

	if (ctx->mat == NULL)
	    rt_parse_error(ctx, "unexpected directive, did not start a material");
	if (has_prefix(ctx, "tex_"))
	{
		keyword = rt_keyword(ctx, "tex_");
		ctx->mat->has_texture |= RT_TEX_ALBEDO_BIT;
		ctx->mat->tex_albedo_offset = tex_by_name(world, ctx, keyword);
		rt_free(keyword);
	}
	else
	{
		ctx->mat->albedo = rt_color(ctx);
	}

}

void
	rt_exec_refractive(t_world *world, t_parse_ctx *ctx)
{
	(void) world;
	if (ctx->mat == NULL)
	    rt_parse_error(ctx, "unexpected directive, did not start a material");
	ctx->mat->refractive = 1;
	ctx->mat->reflective = 1;
	ctx->mat->refractive_index = rt_float(ctx);
}

void
	rt_exec_density(t_world *world, t_parse_ctx *ctx)
{
	(void) world;
	if (ctx->mat == NULL)
	    rt_parse_error(ctx, "unexpected directive, did not start a material");
	ctx->mat->density = rt_float(ctx);
}

void
	rt_exec_brightness(t_world *world, t_parse_ctx *ctx)
{
	(void) world;
	if (ctx->mat == NULL)
	    rt_parse_error(ctx, "unexpected directive, did not start a material");
	ctx->mat->brightness = rt_float(ctx);
}

void
	rt_exec_fuzzy(t_world *world, t_parse_ctx *ctx)
{
	(void) world;
	if (ctx->mat == NULL)
	    rt_parse_error(ctx, "unexpected directive, did not start a material");
	ctx->mat->reflective = 1;
	ctx->mat->fuzzy = rt_float(ctx);
}

void
	rt_exec_smooth(t_world *world, t_parse_ctx *ctx)
{
	(void) world;
	if (ctx->mat == NULL)
	    rt_parse_error(ctx, "unexpected directive, did not start a material");
	ctx->mat->is_smooth = 1;
}

void
	rt_exec_mat_end(t_world *world, t_parse_ctx *ctx)
{
	(void) world;
	if (ctx->mat == NULL)
	    rt_parse_error(ctx, "unexpected directive, did not start a material");
	ctx->mat = NULL;
}
