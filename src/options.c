#include "state.h"
#include "util.h"
#include "work.h"
#include "cl_impl.h"

#include <libft.h>
#include <ft_printf.h>
#include <stdlib.h>

#ifndef RT_MAJOR_VER
# define RT_MAJOR_VER 0
#endif
#ifndef RT_MINOR_VER
# define RT_MINOR_VER 0
#endif
#ifndef RT_PATCH_VER
# define RT_PATCH_VER 0
#endif

static void
	usage(const char *name)
{
	ft_printf("USAGE\n");
	ft_printf("\t%s [OPTIONS] PATH\n", name);
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
	ft_printf("\t{-D --mode} MODE        select default render mode\n");
	ft_printf("\t{-h --help}             show this help message\n");
	ft_printf("RENDERERS\n");
	ft_printf("\t1                       render on a single thread (slow)\n");
	ft_printf("\tcpu                     render on multiple threads\n");
	ft_printf("\tgpu                     render on the gpu\n");
	ft_printf("\tnet                     render over the network\n");
	ft_printf("MODES\n");
	ft_printf("\t0                       default render mode\n");
	ft_printf("\t1                       UV render mode\n");
	ft_printf("\t2                       geometric normal render mode\n");
	ft_printf("\t3                       shading normal render mode\n");
	ft_printf("\t4                       albedo render mode\n");
}

/* ODOT: dynamic job allocation */
void
	parse_options(t_options *opts, int argc, char **argv)
{
	int	i;

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
#if RT_BONUS
	opts->backends = RT_BACKEND_THREAD;
	opts->samples = 1048576;
#endif
	i = 1;
	while (i < argc)
	{
		if (ft_strcmp(argv[i], "-o") == 0 || ft_strcmp(argv[i], "--out") == 0)
		{
			rt_assert(opts->image_file == NULL, "more than one output file specified");
			rt_assert(i + 1 < argc, "--out requires an argument");
			opts->image_file = argv[i + 1];
			i += 2;
		}
		else if (ft_strcmp(argv[i], "-s") == 0 || ft_strcmp(argv[i], "--spp") == 0)
		{
			rt_assert(!opts->samples_set, "more than one sample size specified");
			rt_assert(i + 1 < argc, "--spp requires an argument");
			opts->samples = ft_atoi(argv[i + 1]);
			rt_assert(opts->samples > 0, "invalid sample size");
			opts->samples_set = 1;
			i += 2;
		}
		else if (ft_strcmp(argv[i], "--width") == 0)
		{
			rt_assert(!opts->width_set, "more than one width specified");
			rt_assert(i + 1 < argc, "--width requires an argument");
			opts->width = ft_atoi(argv[i + 1]);
			rt_assert(opts->width > 0, "invalid width");
			opts->width_set = 1;
			i += 2;
		}
		else if (ft_strcmp(argv[i], "--height") == 0)
		{
			rt_assert(!opts->height_set, "more than one height specified");
			rt_assert(i + 1 < argc, "--height requires an argument");
			opts->height = ft_atoi(argv[i + 1]);
			rt_assert(opts->height > 0, "invalid height");
			opts->height_set = 1;
			i += 2;
		}
		else if (ft_strcmp(argv[i], "-r") == 0 || ft_strcmp(argv[i], "--render") == 0)
		{
			rt_assert(i + 1 < argc, "--render requires an argument");
			if (!opts->backends_set)
				opts->backends = 0;
			if (ft_strcmp(argv[i + 1], "1") == 0)
				opts->backends |= RT_BACKEND_SINGLE;
			else if (ft_strcmp(argv[i + 1], "cpu") == 0 && RT_BONUS)
				opts->backends |= RT_BACKEND_THREAD;
			else if (ft_strcmp(argv[i + 1], "gpu") == 0 && RT_BONUS)
				opts->backends |= RT_BACKEND_OPENCL;
			else if (ft_strcmp(argv[i + 1], "net") == 0 && RT_BONUS)
				opts->backends |= RT_BACKEND_SERVER;
			else
				rt_assert(0, "invalid rendering backend");
			opts->backends_set = 1;
			i += 2;
		}
		else if (ft_strcmp(argv[i], "-i") == 0 || ft_strcmp(argv[i], "--ip") == 0)
		{
			rt_assert(!opts->net_ip_set, "more than one ip address specified");
			rt_assert(i + 1 < argc, "--ip requires an argument");
			opts->net_ip = argv[i + 1];
			opts->net_ip_set = 1;
			i += 2;
		}
		else if (ft_strcmp(argv[i], "-p") == 0 || ft_strcmp(argv[i], "--port") == 0)
		{
			rt_assert(!opts->net_port_set, "more than one port specified");
			rt_assert(i + 1 < argc, "--port requires an argument");
			opts->net_port = argv[i + 1];
			opts->net_port_set = 1;
			i += 2;
		}
		else if (ft_strcmp(argv[i], "-j") == 0 || ft_strcmp(argv[i], "--net-jobs") == 0)
		{
			rt_assert(!opts->net_jobs_set, "more than one net job count specified");
			rt_assert(i + 1 < argc, "--net-jobs requires an argument");
			opts->net_jobs = ft_atoi(argv[i + 1]);
			rt_assert(opts->net_jobs > 0 && opts->net_jobs <= 1024, "invalid net job count");
			opts->net_jobs_set = 1;
			i += 2;
		}
		else if (ft_strcmp(argv[i], "-J") == 0 || ft_strcmp(argv[i], "--req-jobs") == 0)
		{
			rt_assert(!opts->req_jobs_set, "more than one request job count specified");
			rt_assert(i + 1 < argc, "--req-jobs requires an argument");
			opts->req_jobs = ft_atoi(argv[i + 1]);
			rt_assert(opts->req_jobs > 0 && opts->req_jobs <= 64, "invalid request job count");
			opts->req_jobs_set = 1;
			i += 2;
		}
		else if (ft_strcmp(argv[i], "-W") == 0 || ft_strcmp(argv[i], "--worker") == 0)
		{
			opts->worker = 1;
			i += 1;
		}
		else if (ft_strcmp(argv[i], "-k") == 0 || ft_strcmp(argv[i], "--scene-key") == 0)
		{
			rt_assert(opts->key == NULL, "more than one key specified");
			rt_assert(i + 1 < argc, "--scene-key requires an argument");
			opts->key = argv[i + 1];
			i += 2;
		}
		else if (ft_strcmp(argv[i], "-t") == 0 || ft_strcmp(argv[i], "--threads") == 0)
		{
			rt_assert(!opts->threads_set, "more than one thread count specified");
			rt_assert(i + 1 < argc, "--threads requires an argument");
			opts->threads = ft_atoi(argv[i + 1]);
			rt_assert(opts->threads > 0, "invalid thread count");
			opts->threads_set = 1;
			i += 2;
		}
		else if (ft_strcmp(argv[i], "-b") == 0 || ft_strcmp(argv[i], "--lbatch") == 0)
		{
			rt_assert(!opts->batch_size_set, "more than one logical batch size specified");
			rt_assert(i + 1 < argc, "--lbatch requires an argument");
			opts->batch_size = ft_atoi(argv[i + 1]);
			rt_assert(opts->batch_size > 0, "invalid logical batch size");
			opts->batch_size_set = 1;
			i += 2;
		}
		else if (ft_strcmp(argv[i], "-B") == 0 || ft_strcmp(argv[i], "--gbatch") == 0)
		{
			rt_assert(!opts->trace_batch_size_set, "more than one global batch size specified");
			rt_assert(i + 1 < argc, "--gbatch requires an argument");
			opts->trace_batch_size = ft_atoi(argv[i + 1]);
			rt_assert(opts->trace_batch_size > 0, "invalid global batch size");
			opts->trace_batch_size_set = 1;
			i += 2;
		}
		else if (ft_strcmp(argv[i], "-u") == 0 || ft_strcmp(argv[i], "--no-flate") == 0)
		{
			opts->compression = 0;
			i += 1;
		}
		else if (ft_strcmp(argv[i], "-d") == 0 || ft_strcmp(argv[i], "--device") == 0)
		{
			rt_assert(opts->cl_device_count < 16, "too many opencl devices specified");
			rt_assert(i + 2 < argc, "--device requires two arguments");
			opts->cl_platforms[opts->cl_device_count] = ft_atoi(argv[i + 1]);
			opts->cl_devices[opts->cl_device_count] = ft_atoi(argv[i + 2]);
			rt_assert(opts->cl_platforms[opts->cl_device_count] >= 0, "invalid opencl platform specified");
			rt_assert(opts->cl_devices[opts->cl_device_count] >= 0, "invalid opencl device specified");
			opts->cl_device_count += 1;
			i += 3;
		}
		else if (ft_strcmp(argv[i], "-v") == 0 || ft_strcmp(argv[i], "--version") == 0)
		{
			ft_printf("v%d.%d.%d\n", RT_PATCH_VER, RT_MINOR_VER, RT_MAJOR_VER);
			exit(EXIT_SUCCESS);
		}
		else if (ft_strcmp(argv[i], "-c") == 0 || ft_strcmp(argv[i], "--compile") == 0)
		{
			rt_assert(i + 2 < argc, "--compile requires two arguments");
			opencl_compile(argv[i + 2], argv[i + 1]);
			exit(EXIT_SUCCESS);
		}
		else if (ft_strcmp(argv[i], "-C") == 0 || ft_strcmp(argv[i], "--link") == 0)
		{
			rt_assert(i + 2 < argc, "--link requires at least two arguments");
			opencl_link((const char **) argv + i + 2, argv[i + 1]);
			exit(EXIT_SUCCESS);
		}
		else if (ft_strcmp(argv[i], "-D") == 0 || ft_strcmp(argv[i], "--mode") == 0)
		{
			rt_assert(i + 1 < argc, "--mode requires an argument");
			opts->default_rendering_mode = ft_atoi(argv[i + 1]);
			rt_assert(opts->default_rendering_mode < RT_RENDER_MODE_COUNT, "invalid rendering mode");
			i += 2;
		}
		else if (ft_strcmp(argv[i], "-h") == 0 || ft_strcmp(argv[i], "--help") == 0)
		{
			usage(argv[0]);
			exit(EXIT_SUCCESS);
		}
		else
		{
			rt_assert(opts->scene_file == NULL, "more than one scene file specified");
			opts->scene_file = argv[i];
			i += 1;
		}
	}
	if (!opts->worker)
	{
		rt_assert(opts->scene_file != NULL, "no scene file specified");
	}
	else
	{
		rt_assert(opts->scene_file == NULL, "cannot specify scene file in worker mode");
		rt_assert(~opts->backends & RT_BACKEND_SINGLE, "single threaded backend not allowed in worker mode");
	}
	if (opts->key == NULL)
		opts->key = "default";
}
