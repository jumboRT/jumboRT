#include "work.h"

#include "util.h"

void
	work_create(t_work *work, t_state *state, t_options *opts, union u_client *client)
{
	work->state = state;
	work->workers = NULL;
	work->count = 0;
	work->work_index = 0;
	work->work_size = 0;
	work->work_progress = 0;
	mutex_init(&work->mtx);
	cond_init(&work->cnd);
	work->pause = 0;
	work->stop = 0;
	work->paused = 0;
	work->stopped = 0;
	work->data = NULL;
	work->capacity = 0;
	work->opts = opts;
	work->pending = NULL;
	work->pending_size = 0;
	work->pending_capacity = 0;
	work->client = client;
	work->update_stop = 0;
	work->update_flag = 0;
	cond_init(&work->update_cnd);
	cond_init(&work->progress_cnd);
	mutex_init(&work->state_mtx);
	mutex_init(&work->update_mtx);
	work_int_create(work);
	work_pause(work);
}

void
	work_destroy(t_work *work)
{
	size_t	i;

	mutex_lock(&work->mtx);
	work->stop = 1;
	cond_broadcast(&work->cnd);
	while (work->stopped < work->count)
		cond_wait(&work->cnd, &work->mtx);
	mutex_unlock(&work->mtx);
	i = 0;
	while (i < work->count)
	{
		thread_join(&work->workers[i]->thread);
		queue_destroy(&work->workers[i]->queue);
		i += 1;
	}
	work_int_destroy(work);
	i = 0;
	while (i < work->count)
	{
		rt_free(work->workers[i]);
		i += 1;
	}
	rt_free(work->workers);
	rt_free(work->data);
	mutex_destroy(&work->mtx);
	cond_destroy(&work->cnd);
}

void
	work_resume(t_work *work)
{
	work_int_resume(work);
	mutex_lock(&work->mtx);
	work->pause = 0;
	cond_broadcast(&work->cnd);
	mutex_unlock(&work->mtx);
}

void
	work_pause(t_work *work)
{
	mutex_lock(&work->mtx);
	work->pause = 1;
	while (work->paused < work->count)
		cond_wait(&work->cnd, &work->mtx);
	mutex_unlock(&work->mtx);
}

