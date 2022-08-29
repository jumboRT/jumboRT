#include "pool.h"

#ifdef RT_MT
#include "util.h"

static void
	*pool_create_int(void *ctx)
{
	t_task_item	item;
	t_pool		*pool;
	size_t		new_size;

	pool = ctx;
	mutex_lock(&pool->mtx);
	while (1)
	{
		while (pool->size < sizeof(item) && !pool->stop)
			cond_wait(&pool->cnd, &pool->mtx);
		if (pool->stop)
			break ;
		new_size = pool->size - sizeof(item);
		rt_memcpy(&item, (char *) pool->data + new_size, sizeof(item));
		pool->size = new_size;
		mutex_unlock(&pool->mtx);
		item.task->start(item.task->ctx, item.id);
		mutex_lock(&pool->mtx);
		item.task->done += 1;
		if (item.task->done == item.task->count)
		{
			if (item.task->detached)
				rt_free(item.task);
			else
				cond_broadcast(&pool->cnd);
		}
	}
	mutex_unlock(&pool->mtx);
	return (NULL);
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
	cond_broadcast(&pool->cnd);
	mutex_unlock(&pool->mtx);
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
	pool_fork(t_pool *pool, t_task *task)
{
	t_task_item	item;
	size_t		old_size;
	size_t		new_size;

	mutex_lock(&pool->mtx);
	item.task = task;
	item.id = task->count;
	task->count += 1;
	old_size = pool->size;
	new_size = old_size + sizeof(item);
	rt_assert(new_size / sizeof(item) <= RT_POOL_MAX_TASKS, "pool_fork: too many tasks in pool");
	pool->data = rt_reallog(pool->data, &pool->capacity, new_size);
	rt_memcpy((char *) pool->data + old_size, &item, sizeof(item));
	pool->size = new_size;
	cond_broadcast(&pool->cnd);
	mutex_unlock(&pool->mtx);
}

void
	pool_join(t_pool *pool, t_task *task)
{
	mutex_lock(&pool->mtx);
	while (task->done < task->count)
		cond_wait(&pool->cnd, &pool->mtx);
	mutex_unlock(&pool->mtx);
}

void
	pool_detach(t_pool *pool, t_task *task)
{
	mutex_lock(&pool->mtx);
	task->detached = 1;
	if (task->done >= task->count)
		rt_free(task);
	mutex_unlock(&pool->mtx);
}

#endif

