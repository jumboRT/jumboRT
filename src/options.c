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

/* ODOT: dynamic job allocation */
void
	parse_options(t_options *opts, int argc, char **argv)
{
	int	i;

	opts->scene_file = NULL;
	opts->image_file = NULL;
	opts->samples = 1048576;
	opts->width = 1920;
	opts->height = 1080;
	opts->backends = RT_BACKEND_THREAD;
	opts->worker = 0;
	opts->net_ip = "localhost";
	opts->net_port = "29300";
	opts->net_jobs = 64;
	opts->req_jobs = 4;
	opts->key = NULL;
	opts->threads = 8;
	opts->batch_size = 16;
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
			rt_assert(i + 1 < argc, "-s requires an argument");
			opts->samples = ft_atoi(argv[i + 1]);
			rt_assert(opts->samples > 0, "invalid sample size");
			opts->samples_set = 1;
			i += 2;
		}
		else if (ft_strcmp(argv[i], "-w") == 0)
		{
			rt_assert(!opts->width_set, "more than one width specified");
			rt_assert(i + 1 < argc, "-w requires an argument");
			opts->width = ft_atoi(argv[i + 1]);
			rt_assert(opts->width > 0, "invalid width");
			opts->width_set = 1;
			i += 2;
		}
		else if (ft_strcmp(argv[i], "-h") == 0)
		{
			rt_assert(!opts->height_set, "more than one height specified");
			rt_assert(i + 1 < argc, "-h requires an argument");
			opts->height = ft_atoi(argv[i + 1]);
			rt_assert(opts->height > 0, "invalid height");
			opts->height_set = 1;
			i += 2;
		}
		else if (ft_strcmp(argv[i], "-r") == 0)
		{
			rt_assert(i + 1 < argc, "-r requires an argument");
			if (!opts->backends_set)
				opts->backends = 0;
			if (ft_strcmp(argv[i + 1], "1") == 0)
				opts->backends |= RT_BACKEND_SINGLE;
			else if (ft_strcmp(argv[i + 1], "cpu") == 0)
				opts->backends |= RT_BACKEND_THREAD;
			else if (ft_strcmp(argv[i + 1], "gpu") == 0)
				opts->backends |= RT_BACKEND_OPENCL;
			else if (ft_strcmp(argv[i + 1], "net") == 0)
				opts->backends |= RT_BACKEND_SERVER;
			else
				rt_assert(0, "invalid rendering backend");
			opts->backends_set = 1;
			i += 2;
		}
		else if (ft_strcmp(argv[i], "-i") == 0)
		{
			rt_assert(!opts->net_ip_set, "more than one ip address specified");
			rt_assert(i + 1 < argc, "-i requires an argument");
			opts->net_ip = argv[i + 1];
			opts->net_ip_set = 1;
			i += 2;
		}
		else if (ft_strcmp(argv[i], "-p") == 0)
		{
			rt_assert(!opts->net_port_set, "more than one port specified");
			rt_assert(i + 1 < argc, "-p requires an argument");
			opts->net_port = argv[i + 1];
			opts->net_port_set = 1;
			i += 2;
		}
		else if (ft_strcmp(argv[i], "-j") == 0)
		{
			rt_assert(!opts->net_jobs_set, "more than one net job count specified");
			rt_assert(i + 1 < argc, "-j requires an argument");
			opts->net_jobs = ft_atoi(argv[i + 1]);
			rt_assert(opts->net_jobs > 0 && opts->net_jobs <= 1024, "invalid net job count");
			opts->net_jobs_set = 1;
			i += 2;
		}
		else if (ft_strcmp(argv[i], "-J") == 0)
		{
			rt_assert(!opts->req_jobs_set, "more than one request job count specified");
			rt_assert(i + 1 < argc, "-J requires an argument");
			opts->req_jobs = ft_atoi(argv[i + 1]);
			rt_assert(opts->req_jobs > 0 && opts->req_jobs <= 64, "invalid request job count");
			opts->req_jobs_set = 1;
			i += 2;
		}
		else if (ft_strcmp(argv[i], "-W") == 0)
		{
			opts->worker = 1;
			i += 1;
		}
		else if (ft_strcmp(argv[i], "-k") == 0)
		{
			rt_assert(opts->key == NULL, "more than one key specified");
			rt_assert(i + 1 < argc, "-k requires an argument");
			opts->key = argv[i + 1];
			i += 2;
		}
		else if (ft_strcmp(argv[i], "-t") == 0)
		{
			rt_assert(!opts->threads_set, "more than one thread count specified");
			rt_assert(i + 1 < argc, "-t requires an argument");
			opts->threads = ft_atoi(argv[i + 1]);
			rt_assert(opts->threads > 0, "invalid thread count");
			opts->threads_set = 1;
			i += 2;
		}
		else if (ft_strcmp(argv[i], "-b") == 0)
		{
			rt_assert(!opts->batch_size_set, "more than one batch size specified");
			rt_assert(i + 1 < argc, "-b requires an argument");
			opts->batch_size = ft_atoi(argv[i + 1]);
			rt_assert(opts->batch_size > 0, "invalid batch size");
			opts->batch_size_set = 1;
			i += 2;
		}
		else if (ft_strcmp(argv[i], "-B") == 0)
		{
			rt_assert(!opts->trace_batch_size_set, "more than one trace batch size specified");
			rt_assert(i + 1 < argc, "-B requires an argument");
			opts->trace_batch_size = ft_atoi(argv[i + 1]);
			rt_assert(opts->trace_batch_size > 0, "invalid trace batch size");
			opts->trace_batch_size_set = 1;
			i += 2;
		}
		else if (ft_strcmp(argv[i], "-u") == 0)
		{
			opts->compression = 0;
			i += 1;
		}
		else if (ft_strcmp(argv[i], "-d") == 0)
		{
			rt_assert(opts->cl_device_count < 16, "too many opencl devices specified");
			rt_assert(i + 2 < argc, "-d requires two arguments");
			opts->cl_platforms[opts->cl_device_count] = ft_atoi(argv[i + 1]);
			opts->cl_devices[opts->cl_device_count] = ft_atoi(argv[i + 2]);
			rt_assert(opts->cl_platforms[opts->cl_device_count] >= 0, "invalid opencl platform specified");
			rt_assert(opts->cl_devices[opts->cl_device_count] >= 0, "invalid opencl device specified");
			opts->cl_device_count += 1;
			i += 3;
		}
		else if (ft_strcmp(argv[i], "-v") == 0)
		{
			ft_printf("v%d.%d.%d\n", RT_PATCH_VER, RT_MINOR_VER, RT_MAJOR_VER);
			exit(EXIT_SUCCESS);
		}
		else if (ft_strcmp(argv[i], "-c") == 0)
		{
			rt_assert(i + 2 < argc, "-c requires two arguments");
			opencl_compile(argv[i + 2], argv[i + 1]);
			exit(EXIT_SUCCESS);
		}
		else if (ft_strcmp(argv[i], "-C") == 0)
		{
			rt_assert(i + 2 < argc, "-C requires at least two arguments");
			opencl_link((const char **) argv + i + 2, argv[i + 1]);
			exit(EXIT_SUCCESS);
		}
		else if (ft_strcmp(argv[i], "-D") == 0)
		{
			rt_assert(i + 1 < argc, "-D requires an argument");
			opts->default_rendering_mode = ft_atoi(argv[i + 1]);
			rt_assert(opts->default_rendering_mode < RT_RENDER_MODE_COUNT, "invalid rendering mode");
			i += 2;
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
