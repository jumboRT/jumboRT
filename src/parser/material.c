#include "parser.h"

#include <libft.h>

static const t_material_entry	g_entries[] = {
	{ "lambertian", rt_lambertian },
	{ "metal", rt_metal },
	{ "dielectric", rt_dielectric },
	{ "emitter", rt_emitter },
	{ NULL, NULL }
};

t_material
	*rt_material(const char **line, char **error)
{
	size_t					index;
	const t_material_entry	*entry;

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
			return (entry->proc(line, error));
		}
		index += 1;
	}
	if (*line == NULL)
		return (NULL);
	return (rt_lambertian(line, error));
}

t_material
	*rt_lambertian(const char **line, char **error)
{
	t_lambertian	lambertian;

	lambertian.base.vt = lambertian_vt();
	*line = rt_color(*line, error, &lambertian.albedo);
	if (*line == NULL)
		return (NULL);
	return (rt_memdup(&lambertian, sizeof(lambertian)));
}

t_material
	*rt_metal(const char **line, char **error)
{
	t_metal	metal;

	metal.base.vt = metal_vt();
	*line = rt_color(*line, error, &metal.albedo);
	*line = rt_float(*line, error, &metal.fuzzy);
	if (*line == NULL)
		return (NULL);
	return (rt_memdup(&metal, sizeof(metal)));
}

t_material
	*rt_dielectric(const char **line, char **error)
{
	t_dielectric	dielectric;

	dielectric.base.vt = dielectric_vt();
	*line = rt_float(*line, error, &dielectric.ir);
	if (*line == NULL)
		return (NULL);
	return (rt_memdup(&dielectric, sizeof(dielectric)));
}

t_material
	*rt_emitter(const char **line, char **error)
{
	t_emitter	emitter;

	emitter.base.vt = emitter_vt();
	*line = rt_float(*line, error, &emitter.brightness);
	*line = rt_color(*line, error, &emitter.emittance);
	emitter.child = rt_material(line, error);
	if (*line == NULL)
		return (NULL);
	return (rt_memdup(&emitter, sizeof(emitter)));
}

