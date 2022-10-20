#ifndef STATE_H
# define STATE_H

# include "rtmath.h"
# include "world.h"
# include "types.h"
# include "mt.h"
# include "image.h"
# ifndef RT_JOINC
#  include "gfx.h"
# endif

# include <stddef.h>
# include <signal.h>

# if RT_BONUS
#  define RT_BONUS_OPTIONS 1
# else
#  define RT_BONUS_OPTIONS 0
# endif

typedef struct s_state		t_state;
typedef struct s_options	t_options;

# ifdef RT_JOINC

typedef int					t_win;

# endif

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

struct s_state {
	t_image			*image;
	t_world			*world;
	t_win			win;
};

int		parse_option_1(t_options *opts, int argc, char **argv, int *i);
int		parse_option_2(t_options *opts, int argc, char **argv, int *i);
int		parse_option_3(t_options *opts, int argc, char **argv, int *i);
int		parse_option_4(t_options *opts, int argc, char **argv, int *i);
int		parse_option_5(t_options *opts, int argc, char **argv, int *i);
int		parse_option_6(t_options *opts, int argc, char **argv, int *i);
int		parse_option_7(t_options *opts, int argc, char **argv, int *i);
int		parse_option_8(t_options *opts, int argc, char **argv, int *i);
int		parse_option_9(t_options *opts, int argc, char **argv, int *i);

int		parse_option(t_options *opts, int argc, char **argv, int *i);
void	parse_options(t_options *opts, int argc, char **argv);

#endif
