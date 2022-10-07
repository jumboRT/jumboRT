#include "parser.h"

#include "mat.h"
#include "util.h"
#include "shape.h"
#include "world_impl.h"
#include <libft.h>

int
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
	uint32_t	result;

	if (has_prefix(ctx, "tex_"))
	{
		keyword = rt_keyword(ctx, "tex_");
		result = tex_by_name(world, ctx, keyword);
		rt_free(keyword);
	}
	else
	{
		result = tex_by_color(world, ctx, rt_color(ctx));
	}
	return (result);
}

t_filter
	rt_filter(t_world *world, t_parse_ctx *ctx)
{
	t_filter	result;
	size_t		len;

	rt_skip(ctx, ft_isspace);
	len = rt_idlen(ctx);
	if (6 == len && ft_strncmp("filter", ctx->data, len) == 0)
	{
		rt_idskip(ctx, len);
		result.tex[0] = rt_texture(world, ctx);
		result.tex[1] = rt_texture(world, ctx);
	}
	else
	{
		result.tex[0] = rt_texture(world, ctx);
		result.tex[1] = tex_by_color(world, ctx, vec(1.0, 1.0, 1.0, 1.0));
	}
	return (result);
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
	bxdf.base.weight = rt_float(ctx);
	bxdf.base.tex = rt_filter(world, ctx);
	world_insert_bxdf(world, ctx->mat, &bxdf, sizeof(bxdf));
}

void
	rt_exec_reflective(t_world *world, t_parse_ctx *ctx)
{
	t_bxdf_reflective	bxdf;

	if (ctx->mat == NULL)
	    rt_parse_error(ctx, "unexpected directive, did not start a material");
	bxdf.base.type = RT_BXDF_REFLECTIVE;
	bxdf.base.weight = rt_float(ctx);
	bxdf.base.tex = rt_filter(world, ctx);
	world_insert_bxdf(world, ctx->mat, &bxdf, sizeof(bxdf));
}

void
	rt_exec_cook_torrance(t_world *world, t_parse_ctx *ctx)
{
	t_bxdf_cook_torrance bxdf;

	if (ctx->mat == NULL)
	    rt_parse_error(ctx, "unexpected directive, did not start a material");
	bxdf.base.type = RT_BXDF_COOK_TORRANCE;
	bxdf.base.weight = rt_float(ctx);
	bxdf.roughness = rt_float(ctx);
	bxdf.k = rt_float(ctx);
	bxdf.base.tex = rt_filter(world, ctx);
	world_insert_bxdf(world, ctx->mat, &bxdf, sizeof(bxdf));

}

void
	rt_exec_bphong(t_world *world, t_parse_ctx *ctx)
{
	t_bxdf_bphong	bxdf;

	if (ctx->mat == NULL)
	    rt_parse_error(ctx, "unexpected directive, did not start a material");
	bxdf.base.type = RT_BXDF_BLINN_PHONG;
	bxdf.base.weight = rt_float(ctx);
	bxdf.base.tex = rt_filter(world, ctx);
	bxdf.spec = rt_filter(world, ctx);
	bxdf.alpha = rt_filter(world, ctx);
	world_insert_bxdf(world, ctx->mat, &bxdf, sizeof(bxdf));
}

void
	rt_exec_specular(t_world *world, t_parse_ctx *ctx)
{
	t_bxdf_specular	bxdf;

	if (ctx->mat == NULL)
	    rt_parse_error(ctx, "unexpected directive, did not start a material");
	bxdf.base.weight = rt_float(ctx);
	bxdf.base.type = RT_BXDF_SPECULAR;
	bxdf.base.tex = rt_filter(world, ctx);
	world_insert_bxdf(world, ctx->mat, &bxdf, sizeof(bxdf));
}

void
	rt_exec_transmissive(t_world *world, t_parse_ctx *ctx)
{
	t_bxdf_transmissive	bxdf;

	if (ctx->mat == NULL)
	    rt_parse_error(ctx, "unexpected directive, did not start a material");
	bxdf.base.weight = rt_float(ctx);
	bxdf.base.type = RT_BXDF_TRANSMISSIVE;
	bxdf.base.tex = rt_filter(world, ctx);
	world_insert_bxdf(world, ctx->mat, &bxdf, sizeof(bxdf));
}

void
	rt_exec_emission(t_world *world, t_parse_ctx *ctx)
{
	if (ctx->mat == NULL)
	    rt_parse_error(ctx, "unexpected directive, did not start a material");
	ctx->mat->brightness = rt_float(ctx);
	ctx->mat->emission = rt_filter(world, ctx);
	ctx->mat->flags |= RT_MAT_EMITTER;
}

void
	rt_exec_emission_exp(t_world *world, t_parse_ctx *ctx)
{
	(void) world;
	if (ctx->mat == NULL)
	    rt_parse_error(ctx, "unexpected directive, did not start a material");
	ctx->mat->emission_exp = rt_float(ctx);
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
	if (ctx->mat == NULL)
	    rt_parse_error(ctx, "unexpected directive, did not start a material");
	ctx->mat->alpha = rt_filter(world, ctx);
	ctx->mat->flags |= RT_MAT_HAS_ALPHA;
}

void
	rt_exec_normal(t_world *world, t_parse_ctx *ctx)
{
	(void) world;
	if (ctx->mat == NULL)
	    rt_parse_error(ctx, "unexpected directive, did not start a material");
	ctx->mat->normal_map = rt_texture(world, ctx);
	ctx->mat->flags |= RT_MAT_HAS_NORMAL;
}

void
	rt_exec_bump(t_world *world, t_parse_ctx *ctx)
{
	(void) world;
	if (ctx->mat == NULL)
	    rt_parse_error(ctx, "unexpected directive, did not start a material");
	ctx->mat->bump_map = rt_texture(world, ctx);
	ctx->mat->flags |= RT_MAT_HAS_BUMP;
}

void
	rt_exec_volume(t_world *world, t_parse_ctx *ctx)
{
	(void) world;
	if (ctx->mat == NULL)
	    rt_parse_error(ctx, "unexpected directive, did not start a material");
	ctx->mat->density = rt_float(ctx);
	ctx->mat->flags |= RT_MAT_HAS_VOLUME;
	ctx->mat->volume.begin = world->bxdfs_count;
	ctx->mat->volume.end = world->bxdfs_count;
}

void
	rt_exec_refractive_index(t_world *world, t_parse_ctx *ctx)
{
	(void) world;
	if (ctx->mat == NULL)
	    rt_parse_error(ctx, "unexpected directive, did not start a material");
	ctx->mat->refractive_index = rt_float(ctx);
}


void
	rt_exec_mat_end(t_world *world, t_parse_ctx *ctx)
{
	(void) world;
	if (ctx->mat == NULL)
	    rt_parse_error(ctx, "unexpected directive, did not start a material");
	ctx->mat = NULL;
}
