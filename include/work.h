#ifndef WORK_H
# define WORK_H

# include "state.h"
# include "mt.h"
# include "queue.h"
# include "work_compute.h"

#define RT_BACKEND_SINGLE 1
#define RT_BACKEND_THREAD 2
#define RT_BACKEND_OPENCL 4

typedef struct s_backend	t_backend;
typedef struct s_worker		t_worker;
typedef struct s_work		t_work;

struct s_worker {
	t_work		*work;
	t_queue		queue;
	t_thread	thread;
	void		*ctx;
	int			backend;
};

struct s_work {
	t_worker	**workers;
	size_t		count;
	t_state		*state;
	uint64_t	work_index;
	uint64_t	work_size;
	uint64_t	work_progress;
	t_mutex		mtx;
	t_cond		cnd;
	int			pause;
	int			stop;
	size_t		paused;
	size_t		stopped;
	t_result	*data;
	size_t		capacity;
	int			backends;
};

void	work_create(t_work *work, t_state *state, int backends);
void	work_destroy(t_work *work);
void	work_resume(t_work *work);
void	work_pause(t_work *work);
void	work_update(t_work *work);
void	work_reset(t_work *work);

void	work_add(t_work *work, t_start start, void *ctx, int backend);
int		work_sync(t_work *work, uint64_t *begin, uint64_t *end, size_t size);
void	work_done(t_work *work, t_result *results, size_t size);

void	work_int_create(t_work *work);
void	work_int_destroy(t_work *work);
void	work_int_resume(t_work *work);
void	work_int_create_single(t_work *work);
void	work_int_destroy_single(t_work *work);
void	work_int_resume_single(t_work *work);
void	work_int_create_thread(t_work *work);
void	work_int_destroy_thread(t_work *work);
void	work_int_resume_thread(t_work *work);
void	work_int_create_opencl(t_work *work);
void	work_int_destroy_opencl(t_work *work);
void	work_int_resume_opencl(t_work *work);

void	ctx_init(t_context *ctx);
void	ctx_destroy(t_context *ctx);

#endif
