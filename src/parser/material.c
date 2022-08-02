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

uint32_t
	rt_texture(t_world *world, t_parse_ctx *ctx)
{
	char		*keyword;

	if (has_prefix(ctx, "tex_"))
	{
		keyword = rt_keyword(ctx, "tex_");
		return (tex_by_name(world, ctx, keyword));
	}
	else
	{
		return (tex_by_color(world, ctx, rt_color(ctx)));
	}
}

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
	rt_exec_diffuse(t_world *world, t_parse_ctx *ctx)
{
	t_bxdf_diffuse	bxdf;

	if (ctx->mat == NULL)
	    rt_parse_error(ctx, "unexpected directive, did not start a material");
	bxdf.base.type = RT_BXDF_DIFFUSE;
	bxdf.base.tex = rt_texture(world, ctx);
	world_insert_bxdf(world, ctx->mat, &bxdf, sizeof(bxdf));
}

void
	rt_exec_reflective(t_world *world, t_parse_ctx *ctx)
{
	t_bxdf_reflective	bxdf;

	if (ctx->mat == NULL)
	    rt_parse_error(ctx, "unexpected directive, did not start a material");
	bxdf.base.type = RT_BXDF_REFLECTIVE;
	bxdf.base.tex = rt_texture(world, ctx);
	bxdf.fuzzy = rt_float(ctx);
	world_insert_bxdf(world, ctx->mat, &bxdf, sizeof(bxdf));
}

void
	rt_exec_mf_reflective(t_world *world, t_parse_ctx *ctx)
{
	t_bxdf_mf_reflection bxdf;

	if (ctx->mat == NULL)
	    rt_parse_error(ctx, "unexpected directive, did not start a material");
	bxdf.base.type = RT_BXDF_MF_REFLECTIVE;
	bxdf.alphax = rt_float(ctx);
	bxdf.alphay = rt_float(ctx);
	bxdf.base.tex = rt_texture(world, ctx);
	world_insert_bxdf(world, ctx->mat, &bxdf, sizeof(bxdf));

}

void
	rt_exec_refractive(t_world *world, t_parse_ctx *ctx)
{
	t_bxdf_refractive	bxdf;

	if (ctx->mat == NULL)
	    rt_parse_error(ctx, "unexpected directive, did not start a material");
	bxdf.base.type = RT_BXDF_REFRACTIVE;
	bxdf.base.tex = rt_texture(world, ctx);
	bxdf.refractive_index = rt_float(ctx);
	world_insert_bxdf(world, ctx->mat, &bxdf, sizeof(bxdf));
}

void
	rt_exec_emission(t_world *world, t_parse_ctx *ctx)
{
	(void) world;
	if (ctx->mat == NULL)
	    rt_parse_error(ctx, "unexpected directive, did not start a material");
	ctx->mat->brightness = rt_float(ctx);
	ctx->mat->emission = rt_texture(world, ctx);
	ctx->mat->flags |= RT_MAT_EMITTER;
}

void
	rt_exec_smooth(t_world *world, t_parse_ctx *ctx)
{
	(void) world;
	if (ctx->mat == NULL)
	    rt_parse_error(ctx, "unexpected directive, did not start a material");
	ctx->mat->flags |= RT_MAT_SMOOTH;
}

void
	rt_exec_alpha(t_world *world, t_parse_ctx *ctx)
{
	(void) world;
	if (ctx->mat == NULL)
	    rt_parse_error(ctx, "unexpected directive, did not start a material");
	ctx->mat->alpha_tex = rt_texture(world, ctx);
	ctx->mat->flags |= RT_MAT_HAS_ALPHA;
}

void
	rt_exec_mat_end(t_world *world, t_parse_ctx *ctx)
{
	(void) world;
	if (ctx->mat == NULL)
	    rt_parse_error(ctx, "unexpected directive, did not start a material");
	ctx->mat = NULL;
}
