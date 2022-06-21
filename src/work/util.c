#include "work.h"

#include "util.h"


#include <stdio.h>

void
	work_add(t_work *work, t_start start, void *ctx)
{
	t_worker	*worker;

	worker = rt_malloc(sizeof(*worker));
	worker->work = work;
	queue_create(&worker->queue);
	worker->ctx = ctx;
	work->workers = rt_realloc(work->workers,
		sizeof(*work->workers) * (work->count + 0),
		sizeof(*work->workers) * (work->count + 1));
	work->workers[work->count] = worker;
	work->count += 1;
	thread_create(&worker->thread, start, worker);
}

int
	work_sync(t_work *work, uint64_t *begin, uint64_t *end, size_t size)
{
	mutex_lock(&work->mtx);
	work->paused += 1;
	cond_broadcast(&work->cnd);
	while (1)
	{
		if (work->stop)
		{
			work->stopped += 1;
			cond_broadcast(&work->cnd);
			mutex_unlock(&work->mtx);
			return (0);
		}
		if (!work->pause && work->work_index < work->work_size)
			break ;
		cond_wait(&work->cnd, &work->mtx);
	}
	work->paused -= 1;
	*begin = work->work_index;
	*end = *begin + size;
	if (*end > work->work_size)
		*end = work->work_size;
	work->work_index = *end;
	mutex_unlock(&work->mtx);
	return (1);
}

void
	work_done(t_work *work, t_result *results, size_t size)
{
	t_pixel	*data;
	size_t	i;

	i = 0;
	while (i < size)
	{
		data = &work->state->image->data[results[i].index];
		data->color = vec_add(data->color, results[i].color);
		data->samples += 1;
		i += 1;
	}
}

