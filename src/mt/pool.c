#include "pool.h"

void
	task_init(t_task *task, t_pool_start start, void *ctx)
{
	task->start = start;
	task->ctx = ctx;
	task->count = 0;
	task->done = 0;
}

#ifndef RT_MT

void
	pool_create(t_pool *pool, size_t count)
{
	(void) pool;
	(void) count;
}

void
	pool_destroy(t_pool *pool)
{
	(void) pool;
}

void
	pool_fork(t_pool *pool, t_task *task)
{
	task->start(task->ctx, task->count);
	task->count += 1;
	task->done += 1;
}

void
	pool_wait(t_pool *pool, t_task *task)
{
	(void) pool;
	(void) task;
}

#endif

