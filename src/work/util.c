/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   util.c                                         #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:09 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:09 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "work.h"

#include "util.h"
#include "net.h"
#include <libft.h>

static void
	work_recv(t_work *work, t_uint64 *begin, t_uint64 *end, size_t size)
{
	if (work->opts->worker)
	{
		*begin = work->pending[0];
		*end = *begin + size;
		work->pending[0] = *end;
		if (*end > work->pending[1])
		{
			*end = work->pending[1];
			work->pending_size -= sizeof(*begin) + sizeof(*end);
			ft_memmove(work->pending, work->pending + 2, work->pending_size);
		}
	}
	else
	{
		*begin = work->work_index;
		*end = *begin + size;
		if (*end > work->work_size)
			*end = work->work_size;
	}
	work->work_index += *end - *begin;
}

void
	work_send(t_work *work, t_uint64 begin, t_uint64 end)
{
	mutex_lock(&work->mtx);
	work->pending_size += sizeof(begin) + sizeof(end);
	work->pending = rt_reallog(work->pending,
			&work->pending_capacity, work->pending_size);
	work->pending[work->pending_size / sizeof(t_uint64) - 2] = begin;
	work->pending[work->pending_size / sizeof(t_uint64) - 1] = end;
	work->work_size += end - begin;
	cond_broadcast(&work->cnd);
	mutex_unlock(&work->mtx);
}

void
	work_add(t_work *work, t_start start, void *ctx, int backend)
{
	t_worker	*worker;

	worker = rt_malloc(sizeof(*worker));
	worker->work = work;
	worker->backend = backend;
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
	work_sync(t_work *work, t_uint64 *begin, t_uint64 *end, size_t size)
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
	work_recv(work, begin, end, size);
	mutex_unlock(&work->mtx);
	return (1);
}

void
	work_done(t_work *work, t_result *results, t_uint64 begin, t_uint64 end)
{
	t_pixel		*data;
	t_uint64	i;
	t_uint64	index;

	if (work->opts->worker)
	{
		if (end != begin)
			rt_send_results(work->client, results, begin, end);
		else
			rt_free(results);
	}
	else
	{
		i = 0;
		while (i < end - begin)
		{
			index = project_index(work->state->world, i + begin);
			data = &work->state->image->data[index];
			data->color = vec_add(data->color, results[i].color);
			data->samples += 1;
			i += 1;
		}
		rt_free(results);
	}
}
