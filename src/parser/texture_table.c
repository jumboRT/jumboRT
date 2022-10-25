/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   texture_table.c                                #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:06 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:06 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

#include "world_impl.h"
#include <stdlib.h>
#include <libft.h>

void
	tex_add(t_parse_ctx *ctx, const char *name, t_uint32 index)
{
	t_entry	new_entry;

	new_entry.index = index;
	new_entry.name = ft_strdup(name);
	vector_push(&ctx->textures, &new_entry);
}

t_uint32
	tex_by_name(t_world *world, t_parse_ctx *ctx, const char *name)
{
	size_t	i;
	t_entry	*entry;

	(void) world;
	i = 0;
	while (i < view_size(ctx->textures.view))
	{
		entry = view_get(ctx->textures.view, i);
		if (entry->name != NULL && ft_strcmp(entry->name, name) == 0)
			return (entry->index);
		i += 1;
	}
	rt_parse_error(ctx, "undefined texture %s", name);
	exit(EXIT_FAILURE);
}

t_uint32
	tex_by_color(t_world *world, t_parse_ctx *ctx, t_vec color)
{
	size_t	i;
	t_entry	*entry;
	t_entry	new_entry;
	t_tex	texture;

	i = 0;
	while (i < view_size(ctx->textures.view))
	{
		entry = view_get(ctx->textures.view, i);
		if (entry->name == NULL && vec_eq(color, entry->color))
			return (entry->index);
		i += 1;
	}
	texture.type = RT_TEX_COLOR;
	texture.a.color = color;
	new_entry.index = world_add_tex(world, &texture);
	new_entry.name = NULL;
	new_entry.color = color;
	vector_push(&ctx->textures, &new_entry);
	return (new_entry.index);
}
