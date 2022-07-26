#include "parser.h"
#include "world_impl.h"

#include <libft.h>
#include <stdlib.h>

void
	mat_add(t_parse_ctx *ctx, const char *name, uint32_t index)
{
	t_mat_entry	new_entry;

	new_entry.index = index;
	new_entry.name = ft_strdup(name);
	vector_push(&ctx->materials, &new_entry);
}

void
	tex_add(t_parse_ctx *ctx, const char *name, uint32_t index)
{
	t_mat_entry	new_entry;

	new_entry.index = index;
	new_entry.name = ft_strdup(name);
	vector_push(&ctx->textures, &new_entry);
}

uint32_t
	mat_by_name(t_world *world, t_parse_ctx *ctx, const char *name)
{
	size_t		i;
	t_mat_entry	*entry;

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

uint32_t
	tex_by_name(t_world *world, t_parse_ctx *ctx, const char *name)
{
	size_t		i;
	t_tex_entry	*entry;

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

uint32_t
	mat_by_color(t_world *world, t_parse_ctx *ctx, t_vec color)
{
	size_t		i;
	t_mat_entry *entry;
	t_mat_entry	new_entry;
	t_material	material;

	i = 0;
	while (i < view_size(ctx->materials.view))
	{
		entry = view_get(ctx->materials.view, i);
		if (entry->name == NULL && vec_eq(color, entry->color))
			return (entry->index);
		i += 1;
	}
	material_init(&material);
	material.albedo = color;
	new_entry.index = world_add_material(world, &material, sizeof(material));
	new_entry.name = NULL;
	new_entry.color = color;
	vector_push(&ctx->materials, &new_entry);
	return (new_entry.index);
}
