#include "state.h"
#include "util.h"

#include <libft.h>

void
	parse_options(t_options *opts, int argc, char **argv)
{
	int	i;

	opts->scene_file = NULL;
	opts->image_file = NULL;
	i = 1;
	while (i < argc)
	{
		if (ft_strcmp(argv[i], "-o") == 0)
		{
			rt_assert(opts->image_file == NULL, "more than one output file specified");
			rt_assert(i + 1 < argc, "-o requires an argument");
			opts->image_file = argv[i + 1];
			i += 2;
		}
		else
		{
			rt_assert(opts->scene_file == NULL, "more than one scene file specified");
			opts->scene_file = argv[i];
			i += 1;
		}
	}
	rt_assert(opts->scene_file != NULL, "no scene file specified");
}
