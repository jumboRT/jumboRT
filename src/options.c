#include "state.h"
#include "util.h"

#include <libft.h>

void
	parse_options(t_options *opts, int argc, char **argv)
{
	int	i;

	opts->scene_file = NULL;
	opts->image_file = NULL;
	opts->samples = 1000000;
	opts->width = 1920;
	opts->height = 1080;
	opts->samples_set = 0;
	opts->width_set = 0;
	opts->height_set = 0;
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
		else if (ft_strcmp(argv[i], "-s") == 0)
		{
			rt_assert(!opts->samples_set, "more than one sample size specified");
			opts->samples = ft_atoi(argv[i + 1]);
			rt_assert(opts->samples > 0, "invalid sample size");
			opts->samples_set = 1;
			i += 2;
		}
		else if (ft_strcmp(argv[i], "-w") == 0)
		{
			rt_assert(!opts->width_set, "more than one width specified");
			opts->width = ft_atoi(argv[i + 1]);
			rt_assert(opts->width > 0, "invalid width");
			opts->width_set = 1;
			i += 2;
		}
		else if (ft_strcmp(argv[i], "-h") == 0)
		{
			rt_assert(!opts->height_set, "more than one height specified");
			opts->height = ft_atoi(argv[i + 1]);
			rt_assert(opts->height > 0, "invalid height");
			opts->height_set = 1;
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
