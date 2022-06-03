#include "parser.h"

#include "util.h"
#include <libft.h>

t_entity
	*rt_ambient_light(const char **line, char **error)
{
	t_ambient_light	light;

	*line = rt_float(*line, error, &light.ratio);
	*line = rt_color(*line, error, &light.color);
	if (*line == NULL)
		return (NULL);
	light.base.vt = ambient_light_vt();
	return (rt_memdup(&light, sizeof(light)));
}

t_entity
	*rt_light(const char **line, char **error)
{
	t_light		light;
	t_emitter	*emitter;

	*line = rt_pos(*line, error, &light.pos);
	*line = rt_float(*line, error, &light.brightness);
	*line = rt_color(*line, error, &light.color);
	if (line == NULL)
		return (NULL);
	light.base.vt = light_vt();
	emitter = rt_malloc(sizeof(*emitter));
	emitter->base.vt = emitter_vt();
	emitter->emittance = light.color;
	emitter->brightness = light.brightness;
	emitter->child = NULL;
	light.mat = (t_material *) emitter;
	return (rt_memdup(&light, sizeof(light)));
}
