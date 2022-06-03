#ifndef RT_H
# define RT_H

/* TODO: lights do not seem to be working at the moment */

# ifndef RT_MT
#  define RT_THREADS 1
#  define RT_ONESHOT 1
# else
#  define RT_ONESHOT 0
#  ifndef RT_THREADS
#   define RT_THREADS 12
#  endif
# endif

# ifndef RT_RENDER_CHUNK_SIZE
#  define RT_RENDER_CHUNK_SIZE 256
# endif

# ifndef RT_SAMPLES
#  define RT_SAMPLES 1024
# endif

# ifndef RT_MAX_DEPTH
#  define RT_MAX_DEPTH 64
# endif

# ifndef RT_RAY_LENGTH
#  define RT_RAY_LENGTH 1000000.0
# endif

# ifndef RT_TIME
#  define RT_TIME_FRAME 1
# else
#  define RT_TIME_FRAME 0
# endif

# define RT_FPS 10

# include "rtmath.h"
# include "mt.h"
# include "gfx.h"
# include "scene.h"
# include "util.h"

typedef struct s_rt_state	t_rt_state;

struct s_rt_state {
	t_win		win;
	t_scene		scene;
	t_mutex		mtx;
	t_cond		cnd;
	t_thread	threads[RT_THREADS];
	size_t		idx;
	size_t		end;
	size_t		width;
	size_t		height;
	size_t		size;
	size_t		*order;
	int			running;
	t_vec		*image;
	long		version;
	int			use_conic;
	double		time;
	t_vec		dbg_line[RT_MAX_DEPTH];
	size_t		dbg_size;
};

t_vec	trace(t_thread_ctx *ctx, t_rt_state *state, int x, int y);
void	trace_debug(t_rt_state *state, int x, int y);
t_vec	project_rotate(t_vec v, FLOAT yaw, FLOAT pitch);
void	project_angles(t_vec v, FLOAT *yaw, FLOAT *pitch);
t_ray	project_frustrum(t_camera *cam, FLOAT x, FLOAT y);
t_ray	project_cone(t_camera *cam, FLOAT x, FLOAT y);
t_ray	project_ray(t_rt_state *state, FLOAT x, FLOAT y);

void	thread_reset(t_rt_state *state);
void	thread_start(t_rt_state *state);
void	thread_stop(t_rt_state *state);

void	render_range(t_thread_ctx *ctx, t_rt_state *state, t_vec *dst, size_t begin, size_t end);
void	render_draw(t_rt_state *state, t_vec *dst, size_t begin, size_t end);

int		rt_exit(void *ctx);
int		rt_key_down(int key, void *ctx);
int		rt_button_down(int button, int x, int y, void *ctx);

double	time_time(void);
void	time_sleep(double time);

#endif
