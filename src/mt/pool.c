#include "pool.h"

#ifndef RT_MT

static void
	pool_run_int(void *ctx)
{
	t_jobs_item	*item;

	item = ctx;
	item->jobs->start(item->jobs->ctx, item->jobs->results, item->id);
}

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
	(void) count;
}

void
	pool_add(t_pool *pool, t_pool_start start, void *ctx)
{
	(void) pool;
	start(ctx);
}

void
	pool_wait(t_pool *pool, t_jobs *jobs)
{
	(void) pool;
	(void) jobs;
}

void
	pool_run(t_pool *pool, t_jobs *jobs)
{
	size_t	i;

	i = 0;
	while (i < jobs->count)
	{
		jobs->items[i].jobs = jobs;
		jobs->items[i].id = i;
		pool_add(pool, pool_run_int, &jobs->items[i]);
		i += 1;
	}
}

#endif

