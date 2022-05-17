#include "parser.h"

#include "util.h"

t_entity
	*rt_ambient_light(const char *line, char **error)
{
	FLOAT				ratio;
	t_vec				color;
	t_ambient_lighting	*light;

	if (!rt_float(line, error, &ratio))
		return (NULL);
	if (!rt_color(line, error, &ratio))
		return (NULL);
	if (!rt_end(line, error))
		return (NULL);
	light = rt_malloc(sizeof(*light));
	light->ratio = ratio;
	light->color = color;
	return (light);
}

t_entity
	*rt_light(const char *line, char **error)
{
	t_vec	pos;
	t_vec	color;
	FLOAT	brightness;
	t_light	*light;

	if (!rt_pos(line, error, &pos))
		return (NULL);
	if (!rt_float(line, error, &brightness))
		return (NULL);
	if (!rt_color(line, error, &color))
		return (NULL);
	light = rt_malloc(sizeof(*light));
	light->pos = pos;
	light->color = color;
	light->brightness = brightness;
	return ((t_entity *) light);
}
