#include "work.h"

#include "util.h"

void
	work_update(t_work *work)
{
	size_t	i;
	size_t	j;
	size_t	n;
	void	*ptr;

	i = 0;
	while (i < work->count)
	{
		ptr = work->data;
		n = queue_recv(&work->workers[i]->queue, &ptr, &work->capacity);
		n /= sizeof(t_result_block);
		work->data = ptr;
		j = 0;
		while (j < n)
		{
			work->work_progress += work->data[j].end - work->data[j].begin;
			work_done(work, work->data[j].results, work->data[j].begin, work->data[j].end);
			j += 1;
		}
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
		while (work->update_flag == 1)
		{
			work->update_flag = 0;
			mutex_unlock(&work->update_mtx);
			mutex_lock(&work->state_mtx);
			work_update(work);
			cond_broadcast(&work->progress_cnd);
			mutex_unlock(&work->state_mtx);
			mutex_lock(&work->update_mtx);
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
	work_send_results(t_worker *worker, t_result *results, uint64_t begin, uint64_t end)
{
	t_result_block	block;

	block.results = rt_memdup(results, (end - begin) * sizeof(*results));
	block.begin = begin;
	block.end = end;
	queue_send(&worker->queue, &block, sizeof(block));
	mutex_lock(&worker->work->update_mtx);
	while (worker->work->update_flag == -1)
		cond_wait(&worker->work->update_cnd, &worker->work->update_mtx);
	worker->work->update_flag = 1;
	cond_broadcast(&worker->work->update_cnd);
	mutex_unlock(&worker->work->update_mtx);
}

