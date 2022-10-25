/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   pool_create_int.c                              #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:19 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:19 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "pool.h"
#include "util.h"

#ifdef RT_MT

static void
	task_done(t_pool *pool, t_task_item *item)
{
	item->task->done += 1;
	if (item->task->done == item->task->count)
	{
		if (item->task->detached)
			rt_free(item->task);
		else
			cond_broadcast(&pool->cnd);
	}
}

void
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
		task_done(pool, &item);
	}
	mutex_unlock(&pool->mtx);
	return (NULL);
}

#endif
