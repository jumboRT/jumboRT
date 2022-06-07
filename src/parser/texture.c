#include "parser.h"

#include <libft.h>

static const t_texture_entry	g_entries[] = {
	{ "image", rt_image_texture },
	{ "color", rt_color_texture },
	{ NULL, NULL }
};

t_texture
	*rt_texture(t_scene *scene, const char **line, char **error)
{
	size_t					index;
	const t_texture_entry	*entry;

	if (*line == NULL)
		return (NULL);
	*line = rt_skip(*line, ft_isspace);
	index = 0;
	while (g_entries[index].identifier != NULL)
	{
		entry = &g_entries[index];
		if (!ft_strncmp(entry->identifier, *line,
				rt_wordlen(entry->identifier)))
		{
			*line = rt_next_word(*line);
			return (entry->proc(scene, line, error));
		}
		index += 1;
	}
	if (*line == NULL)
		return (NULL);
	return (rt_color_texture(scene, line, error));
}

t_texture
	*rt_image_texture(t_scene *scene, const char **line, char **error)
{
	t_image_texture	image;
	size_t			size;
	char			*name;

	image.base.vt = image_texture_vt();
	*line = rt_skip(*line, ft_isspace);
	size = rt_wordlen(*line);
	name = ft_strndup(*line, size);
	*line += size;
	image.tex = tex_load(&scene->texs, name, error);
	rt_free(name);
	if (image.tex == NULL)
	{
		*line = NULL;
		return (NULL);
	}
	return (rt_memdup(&image, sizeof(image)));
}

t_texture
	*rt_color_texture(t_scene *scene, const char **line, char **error)
{
	t_color_texture	color;

	(void) scene;
	color.base.vt = color_texture_vt();
	*line = rt_color(*line, error, &color.color);
	if (*line == NULL)
		return (NULL);
	return (rt_memdup(&color, sizeof(color)));
}

