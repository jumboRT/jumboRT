/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   thread.c                                       #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:19 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:19 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "mt.h"
#include "util.h"

#ifndef RT_MT

void
	thread_create(t_thread *thr, t_start start, void *arg)
{
	*thr = 1;
	start(arg);
}

void
	thread_join(t_thread *thr)
{
	rt_assert(*thr, "cannot join joined thread");
	*thr = 0;
}

void
	thread_priority(t_thread *thr, int prio)
{
	(void) thr;
	(void) prio;
}

void
	thread_exit(void *retval)
{
	(void) retval;
	rt_assert(0, "thread_exit called in single threaded program");
}

#endif
