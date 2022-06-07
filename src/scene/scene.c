#include "scene.h"
#include "tree.h"

#include <stdlib.h>

void
	scene_destroy(t_scene *scene)
{
	size_t		i;
	t_entity	*ent;

	if (scene->camera != NULL)
		camera_vt()->destroy((t_entity *) scene->camera);
	if (scene->ambient_light != NULL)
		ambient_light_vt()->destroy((t_entity *) scene->ambient_light);
	if (scene->main_light != NULL)
		light_vt()->destroy((t_entity *) scene->main_light);
	i = 0;
	while (i < scene->count)
	{
		ent = scene->entities[i];
		i += 1;
		ent->vt->destroy(ent);
	}
	rt_free(scene->entities);
	tree_destroy(scene->tree);
	tex_destroy(&scene->texs);
}
