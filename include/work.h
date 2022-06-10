#ifndef WORK_H
# define WORK_H

# include "state.h"
# include "mt.h"
# include "queue.h"

typedef struct s_result	t_result;
typedef struct s_worker	t_worker;
typedef struct s_work	t_work;

struct s_result {
	size_t	index;
	t_vec	color;
};

struct s_worker {
	t_work		*work;
	t_queue		queue;
	t_thread	thread;
	void		*ctx;
};

struct s_work {
	t_state		*state;
	t_worker	**workers;
	size_t		count;
	size_t		work_index;
	size_t		work_size;
	void		*ctx;
	t_mutex		mtx;
	t_cond		cnd;
	int			pause;
	int			stop;
	size_t		paused;
	size_t		stopped;
	t_result	*data;
	size_t		capacity;
};

void		work_create(t_work *work, t_state *state);
void		work_destroy(t_work *work);
void		work_resume(t_work *work);
void		work_pause(t_work *work);
void		work_update(t_work *work);

void		work_add(t_work *work, t_start start, void *ctx);
int			work_sync(t_work *work, size_t *begin, size_t *end, size_t size);
void		work_done(t_work *work, t_result *results, size_t size);

void		work_int_create(t_work *work);
void		work_int_destroy(t_work *work);
void		work_int_resume(t_work *work);

t_result	work_compute(t_world *world, t_context *ctx, size_t index);

#endif
