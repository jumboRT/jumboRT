#include "parser.h"
#include "tree.h"

#include <libft.h>
#include <ft_printf.h>

static const t_entry	g_entries[] = {
	{ "A", rt_ambient_light },
	{ "C", rt_camera },
	{ "L", rt_light },
	{ "tr", rt_triangle },
	{ "sp", rt_sphere },
	{ "co", rt_cone },
	{ "pl", rt_plane },
	{ "cy", rt_cylinder },
	{ NULL, NULL }
};

t_entity
	*rt_entity(const char **line, char **error)
{
	size_t			index;
	const t_entry	*entry;

	index = 0;
	while (g_entries[index].identifier != NULL)
	{
		entry = &g_entries[index];
		if (!ft_strncmp(entry->identifier, *line,
				rt_wordlen(entry->identifier)))
		{
			*line = rt_next_word(*line);
			return (entry->proc(line, error));
		}
		index += 1;
	}
	ft_asprintf(error, "Unknown object type '%.*s'", rt_wordlen(*line), *line);
	return (NULL);
}

void
	rt_scene_add(t_scene *scene, t_entity *entity)
{
	scene->entities = rt_realloc(scene->entities,
			(scene->count + 0) * sizeof(*scene->entities),
			(scene->count + 1) * sizeof(*scene->entities));
	scene->entities[scene->count++] = entity;
}

int
	rt_scene_one(t_scene *scene, t_entity *entity, char **error)
{
	if (entity->vt == camera_vt())
	{
		if (scene->camera != NULL)
			return (ft_asprintf(error, "Multiple cameras"), 1);
		scene->camera = (t_camera *) entity;
	}
	else if (entity->vt == ambient_light_vt())
	{
		if (scene->ambient_light != NULL)
			return (ft_asprintf(error, "Multiple ambient lights"), 1);
		scene->ambient_light = (t_ambient_light *) entity;
	}
	else if (entity->vt == light_vt())
	{
		if (scene->main_light != NULL)
			return (ft_asprintf(error, "Multiple lights"), 1);
		// TODO: can't set this here cause it will get freed twice
		// scene->main_light = (t_light *) entity;
		rt_scene_add(scene, entity);
	}
	else
		rt_scene_add(scene, entity);
	return (0);
}

int
	rt_scene_all(t_scene *scene, const char *file, char **error)
{
	t_entity	*entity;
	int			sol;

	sol = 1;
	while (*file != '\0')
	{
		if (!sol)
			return (ft_asprintf(error, "Expected newline at end of line"
					", found '%c'", *file), 1);
		entity = rt_entity(&file, error);
		if (entity == NULL || rt_scene_one(scene, entity, error))
			return (1);
		sol = *file == '\n';
		while (*file == '\n')
			file += 1;
	}
	return (0);
}

int
	rt_scene(t_scene *scene, const char *file, char **error)
{
	int			err;

	scene->entities = NULL;
	scene->count = 0;
	scene->camera = NULL;
	scene->ambient_light = NULL;
	scene->main_light = NULL;
	err = rt_scene_all(scene, file, error);
	if (!err && scene->camera == NULL)
		ft_asprintf(error, "Scene has no camera");
	err |= (scene->camera == NULL);
	if (err)
	{
		rt_free(scene->camera);
		rt_free(scene->ambient_light);
		rt_free(scene->main_light);
		while (scene->count > 0)
			rt_free(scene->entities[--scene->count]);
		rt_free(scene->entities);
	}
	else
	{
		scene->tree = tree_new(scene->entities, scene->count);
		tree_optimize(scene->tree, 4);
		/* ft_printf("tree quality * 1000: %d\n", (int) tree_quality(scene->tree) * 1000);
		ft_printf("main tree count: %d\n", (int) scene->tree->count);
		ft_printf("front tree count: %d\n", (int) scene->tree->front->count);
		ft_printf("back tree count: %d\n", (int) scene->tree->back->count);*/
	}
	return (err);
}
