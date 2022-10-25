/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   pool.c                                         #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:19 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:19 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "pool.h"
#include "util.h"

#ifndef RT_MT

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
}

void
	pool_fork(t_pool *pool, t_task *task)
{
	(void) pool;
	task->start(task->ctx, task->count);
	task->count += 1;
	task->done += 1;
}

void
	pool_wait(t_pool *pool, t_task *task)
{
	(void) pool;
	(void) task;
}

void
	pool_detach(t_pool *pool, t_task *task)
{
	(void) pool;
	task->detached = 1;
	rt_free(task);
}

#endif
