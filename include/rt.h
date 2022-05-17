#ifndef RT_H
# define RT_H

# include "rtmath.h"
# include "mt.h"
# include "gfx.h"
# include "scene.h"
# include "util.h"

typedef struct s_rt_state	t_rt_state;

struct s_rt_state {
	t_win	win;
	t_img	img;
	t_scene	scene;
	t_mutex	mtx;
	t_cond	cnd;
	size_t	idx;
	int		running;
};

#endif
