#ifndef STATE_H
# define STATE_H

# include "rtmath.h"
# include "world.h"
# include "cl.h"
# include "gfx.h"
# include "mt.h"
# include "image.h"

# include <stddef.h>
# include <signal.h>

typedef struct s_state		t_state;
typedef struct s_options	t_options;

struct s_options {
	const char	*scene_file;
	const char	*image_file;
};

struct s_state {
	t_image			*image;
	t_world			*world;
	t_win			win;
	t_mutex			mtx;
	int				stop_update;
	sig_atomic_t	should_exit;
	t_thread		work_thread;
};

void	parse_options(t_options *opts, int argc, char **argv);

#endif
