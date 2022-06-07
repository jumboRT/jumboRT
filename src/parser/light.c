#include "parser.h"

#include "util.h"
#include <libft.h>

t_entity
	*rt_ambient_light(t_scene *scene, const char **line, char **error)
{
	t_ambient_light	light;

	(void) scene;
	*line = rt_float(*line, error, &light.ratio);
	*line = rt_color(*line, error, &light.color);
	if (*line == NULL)
		return (NULL);
	light.base.vt = ambient_light_vt();
	return (rt_memdup(&light, sizeof(light)));
}

t_entity
	*rt_light(t_scene *scene, const char **line, char **error)
{
	t_light			light;
	t_emitter		*emitter;
	t_color_texture	*color;

	(void) scene;
	*line = rt_pos(*line, error, &light.pos);
	*line = rt_float(*line, error, &light.brightness);
	*line = rt_color(*line, error, &light.color);
	if (line == NULL)
		return (NULL);
	light.base.vt = light_vt();
	color = rt_malloc(sizeof(*color));
	color->base.vt = color_texture_vt();
	color->color = light.color;
	emitter = rt_malloc(sizeof(*emitter));
	emitter->base.vt = emitter_vt();
	emitter->emittance = (t_texture *) color;
	emitter->brightness = light.brightness;
	emitter->child = NULL;
	light.mat = (t_material *) emitter;
	return (rt_memdup(&light, sizeof(light)));
}
