#include "parser.h"

#include <libft.h>
#include <ft_printf.h>

static const t_entry	g_entries[] = {
	{ "A", rt_ambient_light },
	{ "C", rt_camera },
	{ "L", rt_light },
	{ "sp", rt_sphere },
	{ "pl", rt_plane },
	{ "cy", rt_cylinder },
	{ NULL, NULL }
};

t_entity
	*rt_entity(const char *line, char **error)
{
	size_t		index;
	t_entry		*entry;

	index = 0;
	while (g_entries[index].identifier != NULL)
	{
		entry = &g_entries[index];
		if (!ft_strncmp(entry->identifier, line, rt_wordlen(entry->identifier)))
			return (entry->proc(rt_next_word(line), error));
		index += 1;
	}
	ft_asprintf(error, "Unknown object type '%.*s'", rt_wordlen(line), line);
	return (NULL);
}
