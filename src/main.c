/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   main.c                                         #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:25 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/27 12:53:07 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

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
/* ODOT: ropes heap buffer overflow */
/* ODOT: pre-normalize tangents */

#include <libft.h>
#include <ft_printf.h>
#include <stdlib.h>
#include <unistd.h>

#include <stdio.h>
#include <fenv.h>

#if RT_DEBUG && !RT_MACOS

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
