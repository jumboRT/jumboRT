#include "pool.h"

#ifdef RT_MT
#include "util.h"

static void
	*pool_create_int(void *ctx)
{
	t_pool_item	item;
	t_pool		*pool;
	size_t		new_size;

	pool = ctx;
	while (1)
	{
		mutex_lock(&pool->mtx);
		while (pool->size < sizeof(item) && !pool->stop)
			cond_wait(&pool->cnd, &pool->mtx);
		if (pool->stop)
		{
			mutex_unlock(&pool->mtx);
			return (NULL);
		}
		new_size = pool->size - sizeof(item);
		rt_memcpy(&item, (char *) pool->data + new_size, sizeof(item));
		pool->size = new_size;
		mutex_unlock(&pool->mtx);
		item.start(item.ctx);
	}
}

static void
	pool_run_int(void *ctx)
{
	t_jobs_item	*item;

	item = ctx;
	item->jobs->start(item->jobs->ctx, item->jobs->results, item->id);
	mutex_lock(&item->jobs->pool->mtx);
	item->jobs->done += 1;
	cond_broadcast(&item->jobs->pool->cnd);
	mutex_unlock(&item->jobs->pool->mtx);
}

void
	pool_create(t_pool *pool, size_t count)
{
	size_t	i;

	pool->threads = rt_malloc(sizeof(*pool->threads) * count);
	pool->count = count;
	pool->data = NULL;
	pool->size = 0;
	pool->capacity = 0;
	pool->stop = 0;
	mutex_init(&pool->mtx);
	cond_init(&pool->cnd);
	i = 0;
	while (i < count)
	{
		thread_create(&pool->threads[i], pool_create_int, pool);
		i += 1;
	}
}

void
	pool_destroy(t_pool *pool)
{
	size_t	i;

	mutex_lock(&pool->mtx);
	pool->stop = 1;
	mutex_unlock(&pool->mtx);
	cond_broadcast(&pool->cnd);
	i = 0;
	while (i < pool->count)
	{
		thread_join(&pool->threads[i]);
		i += 1;
	}
	mutex_destroy(&pool->mtx);
	cond_destroy(&pool->cnd);
	rt_free(pool->threads);
	rt_free(pool->data);
}

void
	pool_add(t_pool *pool, t_pool_start start, void *ctx)
{
	t_pool_item	item;
	size_t		old_size;
	size_t		new_size;

	item.start = start;
	item.ctx = ctx;
	mutex_lock(&pool->mtx);
	old_size = pool->size;
	new_size = old_size + sizeof(item);
	pool->data = rt_reallog(pool->data, &pool->capacity, new_size);
	rt_memcpy((char *) pool->data + old_size, &item, sizeof(item));
	pool->size = new_size;
	mutex_unlock(&pool->mtx);
	cond_broadcast(&pool->cnd);
}

void
	pool_wait(t_pool *pool, t_jobs *jobs)
{
	t_pool_item	item;
	size_t		new_size;

	while (1)
	{
		mutex_lock(&pool->mtx);
		while (pool->size < sizeof(item) && jobs->done < jobs->count)
			cond_wait(&pool->cnd, &pool->mtx);
		if (jobs->done >= jobs->count)
		{
			mutex_unlock(&pool->mtx);
			return ;
		}
		new_size = pool->size - sizeof(item);
		rt_memcpy(&item, (char *) pool->data + new_size, sizeof(item));
		pool->size = new_size;
		mutex_unlock(&pool->mtx);
		item.start(item.ctx);
	}
}

void
	pool_run(t_pool *pool, t_jobs *jobs)
{
	size_t	i;

	jobs->pool = pool;
	jobs->done = 0;
	i = 0;
	while (i < jobs->count)
	{
		jobs->items[i].jobs = jobs;
		jobs->items[i].id = i;
		pool_add(pool, pool_run_int, &jobs->items[i]);
		i += 1;
	}
	pool_wait(pool, jobs);
}

#endif

