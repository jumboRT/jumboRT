/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   work.c                                         #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:09 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:09 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "work.h"

#include "util.h"

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
	i = -1;
	while (++i < work->count)
	{
		thread_join(&work->workers[i]->thread);
		queue_destroy(&work->workers[i]->queue);
	}
	work_int_destroy(work);
	i = -1;
	while (++i < work->count)
		rt_free(work->workers[i]);
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

void
	work_reset(t_work *work)
{
	size_t	i;

	if (work->state->image != NULL)
	{
		i = 0;
		while (i < work->state->image->width * work->state->image->height)
		{
			work->state->image->data[i].samples = 0;
			work->state->image->data[i].color = vec(0, 0, 0, 0);
			i += 1;
		}
	}
	work->work_index = 0;
	work->work_progress = 0;
}
