/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   options.c                                      #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:15 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:15 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "state.h"
#include "util.h"
#include "work.h"
#include "cl_impl.h"

#include <libft.h>
#include <ft_printf.h>
#include <stdlib.h>

static void
	usage_options(void)
{
	ft_printf("OPTIONS\n");
	ft_printf("\t{-o --out} PATH         image output path (.ppm, .pfm)\n");
	ft_printf("\t{-s --spp} SAMPLES      number of samples per pixel\n");
	ft_printf("\t--width WIDTH           image width\n");
	ft_printf("\t--height HEIGHT         image height\n");
	ft_printf("\t{-r --render} RENDERER  render device\n");
	ft_printf("\t{-i --ip} ADDRESS       remote address for -W and -r net\n");
	ft_printf("\t{-p --port} PORT        remote port for -W and -r net\n");
	ft_printf("\t{-j --net-jobs} N       # of jobs to send to remote\n");
	ft_printf("\t{-J --req-jobs} N       # of jobs to request from remote\n");
	ft_printf("\t{-W --worker}           start a network worker\n");
	ft_printf("\t{-k --scene-key} KEY    scene preprocessor key\n");
	ft_printf("\t{-t --threads} N        number of threads for -r cpu\n");
	ft_printf("\t{-b --lbatch} N         logical batch size\n");
	ft_printf("\t{-B --gbatch} N         global batch size\n");
	ft_printf("\t{-u --no-flate}         disable compression\n");
	ft_printf("\t{-d --device} PID DID   opencl platform and device id\n");
	ft_printf("\t{-v --version}          print version information\n");
	ft_printf("\t{-c --compile} PATH     invoke opencl compiler\n");
	ft_printf("\t{-C --link} PATH...     invoke opencl linker\n");
	ft_printf("\t--build PATH...         invoke opencl builder\n");
	ft_printf("\t{-D --mode} MODE        select default render mode\n");
	ft_printf("\t{-h --help}             show this help message\n");
}

static void
	usage(const char *name)
{
	ft_printf("USAGE\n");
	ft_printf("\t%s [OPTIONS] PATH\n", name);
	usage_options();
	ft_printf("RENDERERS\n");
	ft_printf("\t1                       render on a single thread (slow)\n");
	ft_printf("\tcpu                     render on multiple threads (bonus)\n");
	ft_printf("\tgpu                     render on the gpu (bonus)\n");
	ft_printf("\tnet                     render over the network (bonus)\n");
	ft_printf("MODES\n");
	ft_printf("\t0                       default render mode\n");
	ft_printf("\t1                       UV render mode\n");
	ft_printf("\t2                       geometric normal render mode\n");
	ft_printf("\t3                       shading normal render mode\n");
	ft_printf("\t4                       albedo render mode\n");
	exit(EXIT_SUCCESS);
}

static void
	init_options_int(t_options *opts)
{
	opts->threads = 8;
	opts->batch_size = 1;
	opts->trace_batch_size = 1;
	opts->compression = 1;
	opts->samples_set = 0;
	opts->width_set = 0;
	opts->height_set = 0;
	opts->backends_set = 0;
	opts->net_ip_set = 0;
	opts->net_port_set = 0;
	opts->net_jobs_set = 0;
	opts->req_jobs_set = 0;
	opts->threads_set = 0;
	opts->batch_size_set = 0;
	opts->trace_batch_size_set = 0;
	opts->cl_device_count = 0;
	opts->default_rendering_mode = RT_RENDER_MODE_DEFAULT;
	if (RT_BONUS_OPTIONS)
	{
		opts->backends = RT_BACKEND_THREAD;
		opts->samples = 1048576;
	}
}

static void
	init_options(t_options *opts)
{
	opts->scene_file = NULL;
	opts->image_file = NULL;
	opts->samples = 1;
	opts->width = 1920;
	opts->height = 1080;
	opts->backends = RT_BACKEND_SINGLE;
	opts->worker = 0;
	opts->net_ip = "localhost";
	opts->net_port = "29300";
	opts->net_jobs = 64;
	opts->req_jobs = 4;
	opts->key = NULL;
	init_options_int(opts);
}

/* ODOT: dynamic job allocation */
void
	parse_options(t_options *opts, int argc, char **argv)
{
	int	i;

	i = 1;
	init_options(opts);
	while (i < argc)
	{
		if (ft_strcmp(argv[i], "-h") == 0 || ft_strcmp(argv[i], "--help") == 0)
			usage(argv[0]);
		else if (!parse_option(opts, argc, argv, &i))
		{
			rt_assert(opts->scene_file == NULL,
				"more than one scene file specified");
			opts->scene_file = argv[i];
			i += 1;
		}
	}
	if (!opts->worker)
		rt_assert(opts->scene_file != NULL, "no scene file specified");
	else
		rt_assert(opts->scene_file == NULL,
			"cannot specify scene file in worker mode");
	if (opts->key == NULL)
		opts->key = "default";
}
