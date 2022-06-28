#ifndef POOL_H
# define POOL_H

# include "mt.h"
# include "queue.h"

# include <stddef.h>

typedef struct s_pool_item	t_pool_item;
typedef struct s_pool		t_pool;
typedef struct s_task		t_task;
typedef struct s_jobs_item	t_jobs_item;
typedef struct s_jobs		t_jobs;
typedef void				(*t_pool_start)(void *ctx);
typedef void				(*t_jobs_start)(void *ctx, void *result, size_t id);

struct s_pool_item {
	t_pool_start	start;
	void			*ctx;
};

struct s_pool {
	t_thread	*threads;
	size_t		count;
	void		*data;
	size_t		size;
	size_t		capacity;
	t_mutex		mtx;
	t_cond		cnd;
	int			stop;
};

struct s_task {
	t_mutex	mtx;
	t_cond	cnd;
	int		done;
};

struct s_jobs_item {
	t_jobs			*jobs;
	size_t			id;
	t_task			task;
};

struct s_jobs {
	t_jobs_start	start;
	void			*ctx;
	t_jobs_item		*items;
	size_t			count;
	void			*results;
	t_pool			*pool;
};

void	pool_create(t_pool *pool, size_t count);
void	pool_destroy(t_pool *pool);
void	pool_add(t_pool *pool, t_pool_start start, void *ctx);
void	pool_wait(t_pool *pool, t_task *task);
void	pool_run(t_pool *pool, t_jobs jobs);

void	task_create(t_task *task);
void	task_destroy(t_task *task);
void	task_wait(t_task *task);
void	task_done(t_task *task);

#endif
