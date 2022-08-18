#include "work.h"

void
	work_update(t_work *work)
{
	size_t	i;
	size_t	n;
	void	*ptr;

	i = 0;
	while (i < work->count)
	{
		ptr = work->data;
		n = queue_recv(&work->workers[i]->queue, &ptr, &work->capacity);
		n /= sizeof(t_result);
		work->work_progress += n;
		work->data = ptr;
		work_done(work, work->data, n);
		i += 1;
	}
}

static void
	*rt_work_start(void *arg)
{
	t_work *work;

	work = arg;
	mutex_lock(&work->update_mtx);
	while (1)
	{
		if (work->update_flag)
		{
			work_update(work);
			cond_broadcast(&work->progress_cnd);
		}
		if (work->update_stop && work->work_progress >= work->work_index)
		{
			mutex_unlock(&work->update_mtx);
			return (NULL);
		}
		cond_wait(&work->update_cnd, &work->update_mtx);
	}
}

void
	work_update_start(t_work *work)
{
	thread_create(&work->update_thread, rt_work_start, work);
}

void
	work_update_stop(t_work *work)
{
	mutex_lock(&work->update_mtx);
	work->update_stop = 1;
	cond_broadcast(&work->update_cnd);
	mutex_unlock(&work->update_mtx);
	thread_join(&work->update_thread);
}

void
	work_send_results(t_worker *worker, t_result *results, size_t count)
{
	queue_send(&worker->queue, results, sizeof(*results) * count);
	mutex_lock(&worker->work->update_mtx);
	worker->work->update_flag = 1;
	cond_broadcast(&worker->work->update_cnd);
	mutex_unlock(&worker->work->update_mtx);
}

