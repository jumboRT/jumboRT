/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   texture.c                                      #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:05 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:05 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

#include "util.h"
#include "world_impl.h"
#include <libft.h>

void
	rt_exec_tex_def(t_world *world, t_parse_ctx *ctx)
{
	t_tex	tex;
	char	*kpce[4];
	size_t	size;

	texture_init(&tex);
	tex.type = RT_TEX_TEXTURE;
	kpce[0] = rt_keyword(ctx, "tex_");
	kpce[1] = rt_word(ctx);
	kpce[2] = rt_readfile(kpce[1], &kpce[3], &size);
	rt_assert(kpce[2] != NULL, kpce[3]);
	world_load_bmp(world, &tex, (unsigned char *) kpce[2], size);
	tex_add(ctx, kpce[0], world_add_tex(world, &tex));
	rt_free(kpce[0]);
	rt_free(kpce[1]);
	rt_free(kpce[2]);
}

void
	rt_exec_tex_def_filtered(t_world *world, t_parse_ctx *ctx)
{
	t_tex	tex;
	char	*kpce[4];
	size_t	size;

	texture_init(&tex);
	tex.type = RT_TEX_FILTERED;
	kpce[0] = rt_keyword(ctx, "tex_");
	kpce[1] = rt_word(ctx);
	kpce[2] = rt_readfile(kpce[1], &kpce[3], &size);
	rt_assert(kpce[2] != NULL, kpce[3]);
	world_load_bmp(world, &tex, (unsigned char *) kpce[2], size);
	tex_add(ctx, kpce[0], world_add_tex(world, &tex));
	rt_free(kpce[0]);
	rt_free(kpce[1]);
	rt_free(kpce[2]);
}

void
	rt_exec_checker_def(t_world *world, t_parse_ctx *ctx)
{
	t_tex	tex;
	char	*keyword;

	texture_init(&tex);
	tex.type = RT_TEX_CHECKER;
	keyword = rt_keyword(ctx, "tex");
	tex.a.checker.odd_color = rt_color(ctx);
	tex.a.checker.even_color = rt_color(ctx);
	tex.a.checker.odd_color = vec_set(tex.a.checker.odd_color, 3,
			rt_float(ctx));
	tex.a.checker.even_color = vec_set(tex.a.checker.even_color, 3,
			rt_float(ctx));
	tex_add(ctx, keyword, world_add_tex(world, &tex));
	rt_free(keyword);
}

t_uint32
	rt_texture(t_world *world, t_parse_ctx *ctx)
{
	char		*keyword;
	t_uint32	result;

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
