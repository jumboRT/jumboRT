#include "main.h"
#include "work.h"
#include "util.h"
#include "world.h"
#include "world_impl.h"
#include "parser.h"
#include "perf.h"
#include "net.h"

#if RT_DEBUG
# include <fenv.h>
#endif

/* ODOT: better image reconstruction */
/* ODOT: texture filtering */
/* ODOT: material hashing to check if scene is the
 * same as last scene in network client */

/* TODO: rays sometimes bounce back into the surface with smooth shading */
/* TODO: bump/normal maps are broken? */
/* TODO: cone... */

#include <libft.h>
#include <ft_printf.h>
#include <stdlib.h>
#include <unistd.h>

#include <stdio.h>

#if RT_DEBUG

void
	enable_fe(void)
{
	feenableexcept(FE_DIVBYZERO | FE_INVALID);
}

#else

void
	enable_fe(void)
{
}

#endif

int
	main(int argc, char **argv)
{
	t_options		options;

	windows_init();
	enable_fe();
	parse_options(&options, argc, argv);
	if (options.worker)
		main_worker(&options);
	else
		main_viewer(&options);
	windows_exit();
	return (EXIT_SUCCESS);
}
