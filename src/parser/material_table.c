/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   material_table.c                               #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:07 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:07 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

#include "mat.h"
#include "world_impl.h"
#include <libft.h>
#include <stdlib.h>

void
	mat_add(t_parse_ctx *ctx, const char *name, t_uint32 index)
{
	t_entry	new_entry;

	new_entry.index = index;
	new_entry.name = ft_strdup(name);
	vector_push(&ctx->materials, &new_entry);
}

t_uint32
	mat_by_name(t_world *world, t_parse_ctx *ctx, const char *name)
{
	size_t		i;
	t_entry		*entry;

	(void) world;
	i = 0;
	while (i < view_size(ctx->materials.view))
	{
		entry = view_get(ctx->materials.view, i);
		if (entry->name != NULL && ft_strcmp(entry->name, name) == 0)
			return (entry->index);
		i += 1;
	}
	rt_parse_error(ctx, "undefined material %s", name);
	exit(EXIT_FAILURE);
}

t_uint32
	mat_by_color(t_world *world, t_parse_ctx *ctx, t_vec color)
{
	size_t			i;
	t_entry			*entry;
	t_entry			new_entry;
	t_material		material;
	t_bxdf_diffuse	bxdf;

	i = 0;
	while (i < view_size(ctx->materials.view))
	{
		entry = view_get(ctx->materials.view, i);
		if (entry->name == NULL && vec_eq(color, entry->color))
			return (entry->index);
		i += 1;
	}
	bxdf.base.type = RT_BXDF_DIFFUSE;
	bxdf.base.tex.tex[0] = tex_by_color(world, ctx, color);
	bxdf.base.tex.tex[1] = tex_by_color(world, ctx, vec(1.0, 1.0, 1.0, 1.0));
	bxdf.base.weight = 1.0;
	material_init(&material, world);
	world_insert_bxdf(world, &material, &bxdf, sizeof(bxdf));
	new_entry.index = world_add_material(world, &material);
	new_entry.name = NULL;
	new_entry.color = color;
	vector_push(&ctx->materials, &new_entry);
	return (new_entry.index);
}
