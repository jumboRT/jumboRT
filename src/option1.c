/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   option1.c                                      #  # #  #   #  ##   ###   */
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

int
	parse_option_1(t_options *opts, int argc, char **argv, int *i)
{
	if (ft_strcmp(argv[*i], "--width") == 0)
	{
		rt_assert(!opts->width_set, "more than one width specified");
		rt_assert(*i + 1 < argc, "--width requires an argument");
		opts->width = ft_atoi(argv[*i + 1]);
		rt_assert(opts->width > 0, "invalid width");
		opts->width_set = 1;
		*i += 2;
	}
	else if (ft_strcmp(argv[*i], "--height") == 0)
	{
		rt_assert(!opts->height_set, "more than one height specified");
		rt_assert(*i + 1 < argc, "--height requires an argument");
		opts->height = ft_atoi(argv[*i + 1]);
		rt_assert(opts->height > 0, "invalid height");
		opts->height_set = 1;
		*i += 2;
	}
	else if (ft_strcmp(argv[*i], "--start-x") == 0)
	{
		rt_assert(!opts->start_x_set, "more than one start x specified");
		rt_assert(*i + 1 < argc, "--start-x requires an argument");
		opts->start_x = ft_atoi(argv[*i + 1]);
		rt_assert(opts->start_x >= 0, "invalid start x");
		opts->start_x_set = 1;
		*i += 2;
	}
	else if (ft_strcmp(argv[*i], "--start-y") == 0)
	{
		rt_assert(!opts->start_y_set, "more than one start x specified");
		rt_assert(*i + 1 < argc, "--start-y requires an argument");
		opts->start_y = ft_atoi(argv[*i + 1]);
		rt_assert(opts->start_y >= 0, "invalid start x");
		opts->start_y_set = 1;
		*i += 2;
	}
	else if (ft_strcmp(argv[*i], "--end-x") == 0)
	{
		rt_assert(!opts->end_x_set, "more than one end x specified");
		rt_assert(*i + 1 < argc, "--end-x requires an argument");
		opts->end_x = ft_atoi(argv[*i + 1]);
		rt_assert(opts->end_x >= 0, "invalid end x");
		opts->end_x_set = 1;
		*i += 2;
	}
	else if (ft_strcmp(argv[*i], "--end-y") == 0)
	{
		rt_assert(!opts->end_y_set, "more than one end x specified");
		rt_assert(*i + 1 < argc, "--end-y requires an argument");
		opts->end_y = ft_atoi(argv[*i + 1]);
		rt_assert(opts->end_y >= 0, "invalid end x");
		opts->end_y_set = 1;
		*i += 2;
	}
	else
		return (0);
	return (1);
}

int
	parse_option_2(t_options *opts, int argc, char **argv, int *i)
{
	if (ft_strcmp(argv[*i], "-o") == 0 || ft_strcmp(argv[*i], "--out") == 0)
	{
		rt_assert(opts->image_file == NULL,
			"more than one output file specified");
		rt_assert(*i + 1 < argc, "--out requires an argument");
		opts->image_file = argv[*i + 1];
		*i += 2;
	}
	else if (ft_strcmp(argv[*i], "-s") == 0
		|| ft_strcmp(argv[*i], "--spp") == 0)
	{
		rt_assert(!opts->samples_set, "more than one sample size specified");
		rt_assert(*i + 1 < argc, "--spp requires an argument");
		opts->samples = ft_atoi(argv[*i + 1]);
		rt_assert(opts->samples > 0, "invalid sample size");
		opts->samples_set = 1;
		*i += 2;
	}
	else
		return (0);
	return (1);
}

int
	parse_option_3(t_options *opts, int argc, char **argv, int *i)
{
	if (ft_strcmp(argv[*i], "-i") == 0 || ft_strcmp(argv[*i], "--ip") == 0)
	{
		rt_assert(!opts->net_ip_set, "more than one ip address specified");
		rt_assert(*i + 1 < argc, "--ip requires an argument");
		opts->net_ip = argv[*i + 1];
		opts->net_ip_set = 1;
		*i += 2;
	}
	else if (ft_strcmp(argv[*i], "-p") == 0 || !ft_strcmp(argv[*i], "--port"))
	{
		rt_assert(!opts->net_port_set, "more than one port specified");
		rt_assert(*i + 1 < argc, "--port requires an argument");
		opts->net_port = argv[*i + 1];
		opts->net_port_set = 1;
		*i += 2;
	}
	else if (ft_strcmp(argv[*i], "--build") == 0)
	{
		rt_assert(*i + 2 < argc, "--build requires at least two arguments");
		opencl_build((const char **) argv + *i + 2, argv[*i + 1]);
		exit(EXIT_SUCCESS);
	}
	else
		return (0);
	return (1);
}

int
	parse_option_4(t_options *opts, int argc, char **argv, int *i)
{
	if (ft_strcmp(argv[*i], "-j") == 0 || !ft_strcmp(argv[*i], "--net-jobs"))
	{
		rt_assert(!opts->net_jobs_set, "more than one net job count specified");
		rt_assert(*i + 1 < argc, "--net-jobs requires an argument");
		opts->net_jobs = ft_atoi(argv[*i + 1]);
		rt_assert(opts->net_jobs > 0 && opts->net_jobs <= 1024,
			"invalid net job count");
		opts->net_jobs_set = 1;
		*i += 2;
	}
	else if (ft_strcmp(argv[*i], "-J") == 0
		|| ft_strcmp(argv[*i], "--req-jobs") == 0)
	{
		rt_assert(!opts->req_jobs_set,
			"more than one request job count specified");
		rt_assert(*i + 1 < argc, "--req-jobs requires an argument");
		opts->req_jobs = ft_atoi(argv[*i + 1]);
		rt_assert(opts->req_jobs > 0 && opts->req_jobs <= 64,
			"invalid request job count");
		opts->req_jobs_set = 1;
		*i += 2;
	}
	else
		return (0);
	return (1);
}

int
	parse_option_5(t_options *opts, int argc, char **argv, int *i)
{
	if (ft_strcmp(argv[*i], "-c") == 0 || ft_strcmp(argv[*i], "--compile") == 0)
	{
		rt_assert(*i + 2 < argc, "--compile requires two arguments");
		opencl_compile(argv[*i + 2], argv[*i + 1]);
		exit(EXIT_SUCCESS);
	}
	else if (ft_strcmp(argv[*i], "-C") == 0
		|| ft_strcmp(argv[*i], "--link") == 0)
	{
		rt_assert(*i + 2 < argc, "--link requires at least two arguments");
		opencl_link((const char **) argv + *i + 2, argv[*i + 1]);
		exit(EXIT_SUCCESS);
	}
	else if (ft_strcmp(argv[*i], "-D") == 0
		|| ft_strcmp(argv[*i], "--mode") == 0)
	{
		rt_assert(*i + 1 < argc, "--mode requires an argument");
		opts->default_rendering_mode = ft_atoi(argv[*i + 1]);
		rt_assert(opts->default_rendering_mode < RT_RENDER_MODE_COUNT,
			"invalid rendering mode");
		*i += 2;
	}
	else
		return (0);
	return (1);
}
