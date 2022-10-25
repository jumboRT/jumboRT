/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   option.c                                       #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:19 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:19 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

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

int
	parse_option_6(t_options *opts, int argc, char **argv, int *i)
{
	if (ft_strcmp(argv[*i], "-r") == 0 || ft_strcmp(argv[*i], "--render") == 0)
	{
		rt_assert(*i + 1 < argc, "--render requires an argument");
		if (!opts->backends_set)
			opts->backends = 0;
		if (ft_strcmp(argv[*i + 1], "1") == 0)
			opts->backends |= RT_BACKEND_SINGLE;
		else if (ft_strcmp(argv[*i + 1], "cpu") == 0 && RT_BONUS_OPTIONS)
			opts->backends |= RT_BACKEND_THREAD;
		else if (ft_strcmp(argv[*i + 1], "gpu") == 0 && RT_BONUS_OPTIONS)
			opts->backends |= RT_BACKEND_OPENCL;
		else if (ft_strcmp(argv[*i + 1], "net") == 0 && RT_BONUS_OPTIONS)
			opts->backends |= RT_BACKEND_SERVER;
		else
			rt_assert(0, "invalid rendering backend");
		opts->backends_set = 1;
		*i += 1;
	}
	else if (ft_strcmp(argv[*i], "-W") == 0 || !ft_strcmp(argv[*i], "--worker"))
		opts->worker = 1;
	else
		return (0);
	*i += 1;
	return (1);
}

int
	parse_option_7(t_options *opts, int argc, char **argv, int *i)
{
	if (ft_strcmp(argv[*i], "-b") == 0 || ft_strcmp(argv[*i], "--lbatch") == 0)
	{
		rt_assert(!opts->batch_size_set,
			"more than one logical batch size specified");
		rt_assert(*i + 1 < argc, "--lbatch requires an argument");
		opts->batch_size = ft_atoi(argv[*i + 1]);
		rt_assert(opts->batch_size > 0, "invalid logical batch size");
		opts->batch_size_set = 1;
		*i += 2;
	}
	else if (ft_strcmp(argv[*i], "-B") == 0
		|| ft_strcmp(argv[*i], "--gbatch") == 0)
	{
		rt_assert(!opts->trace_batch_size_set,
			"more than one global batch size specified");
		rt_assert(*i + 1 < argc, "--gbatch requires an argument");
		opts->trace_batch_size = ft_atoi(argv[*i + 1]);
		rt_assert(opts->trace_batch_size > 0, "invalid global batch size");
		opts->trace_batch_size_set = 1;
		*i += 2;
	}
	else
		return (0);
	return (1);
}

int
	parse_option_8(t_options *opts, int argc, char **argv, int *i)
{
	if (ft_strcmp(argv[*i], "-k") == 0
		|| ft_strcmp(argv[*i], "--scene-key") == 0)
	{
		rt_assert(opts->key == NULL, "more than one key specified");
		rt_assert(*i + 1 < argc, "--scene-key requires an argument");
		opts->key = argv[*i + 1];
		*i += 2;
	}
	else if (ft_strcmp(argv[*i], "-t") == 0
		|| ft_strcmp(argv[*i], "--threads") == 0)
	{
		rt_assert(!opts->threads_set, "more than one thread count specified");
		rt_assert(*i + 1 < argc, "--threads requires an argument");
		opts->threads = ft_atoi(argv[*i + 1]);
		rt_assert(opts->threads > 0, "invalid thread count");
		opts->threads_set = 1;
		*i += 2;
	}
	else
		return (0);
	return (1);
}

int
	parse_option_9(t_options *opts, int argc, char **argv, int *i)
{
	if (ft_strcmp(argv[*i], "-u") == 0 || !ft_strcmp(argv[*i], "--no-flate"))
		opts->compression = 0;
	else if (ft_strcmp(argv[*i], "-d") == 0 || !ft_strcmp(argv[*i], "--device"))
	{
		rt_assert(opts->cl_device_count < 16,
			"too many opencl devices specified");
		rt_assert(*i + 2 < argc, "--device requires two arguments");
		opts->cl_platforms[opts->cl_device_count] = ft_atoi(argv[*i + 1]);
		opts->cl_devices[opts->cl_device_count] = ft_atoi(argv[*i + 2]);
		rt_assert(opts->cl_platforms[opts->cl_device_count] >= 0,
			"invalid opencl platform specified");
		rt_assert(opts->cl_devices[opts->cl_device_count] >= 0,
			"invalid opencl device specified");
		opts->cl_device_count += 1;
		*i += 2;
	}
	else if (!ft_strcmp(argv[*i], "-v") || !ft_strcmp(argv[*i], "--version"))
	{
		ft_printf("v%d.%d.%d\n", RT_PATCH_VER, RT_MINOR_VER, RT_MAJOR_VER);
		exit(EXIT_SUCCESS);
	}
	else
		return (0);
	*i += 1;
	return (1);
}

int
	parse_option(t_options *opts, int argc, char **argv, int *i)
{
	if (parse_option_1(opts, argc, argv, i))
		return (1);
	if (parse_option_2(opts, argc, argv, i))
		return (1);
	if (parse_option_3(opts, argc, argv, i))
		return (1);
	if (parse_option_4(opts, argc, argv, i))
		return (1);
	if (parse_option_5(opts, argc, argv, i))
		return (1);
	if (parse_option_6(opts, argc, argv, i))
		return (1);
	if (parse_option_7(opts, argc, argv, i))
		return (1);
	if (parse_option_8(opts, argc, argv, i))
		return (1);
	if (parse_option_9(opts, argc, argv, i))
		return (1);
	return (0);
}
