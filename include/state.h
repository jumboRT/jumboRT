#ifndef STATE_H
# define STATE_H

# include "rtmath.h"
# include "world.h"
# include "types.h"
# include "mt.h"
# include "image.h"

# include <stddef.h>
# include <signal.h>

typedef struct s_state		t_state;
typedef struct s_options	t_options;

struct s_options {
	const char	*scene_file;
	const char	*image_file;
	long		samples;
	long		width;
	long		height;
	int			backends;
	int			worker;
	const char	*net_ip;
	const char	*net_port;
	long		net_jobs;
	long		req_jobs;
	const char	*key;
	int			threads;
	long		batch_size;
	long		trace_batch_size;
	int			compression;
	int			cl_platforms[16];
	int			cl_devices[16];
	int			samples_set;
	int			width_set;
	int			height_set;
	int			backends_set;
	int			net_ip_set;
	int			net_port_set;
	int			net_jobs_set;
	int			req_jobs_set;
	int			threads_set;
	int			batch_size_set;
	int			trace_batch_size_set;
	int			cl_device_count;
	int			default_rendering_mode;
};

#ifdef RT_JOINC

typedef int	t_win;

#else

# include "gfx.h"

#endif

struct s_state {
	t_image			*image;
	t_world			*world;
	t_win			win;
};

void	parse_options(t_options *opts, int argc, char **argv);

#endif
