/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   task.c                                         #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:19 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:19 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "pool.h"

void
	task_init(t_task *task, t_pool_start start, void *ctx)
{
	task->start = start;
	task->ctx = ctx;
	task->count = 0;
	task->done = 0;
	task->detached = 0;
}
