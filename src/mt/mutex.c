/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   mutex.c                                        #  # #  #   #  ##   ###   */
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
	mutex_init(t_mutex *mtx)
{
	*mtx = 0;
}

void
	mutex_destroy(t_mutex *mtx)
{
	(void) mtx;
}

void
	mutex_lock(t_mutex *mtx)
{
	rt_assert(!*mtx, "deadlock in single threaded code");
	*mtx = 1;
}

void
	mutex_unlock(t_mutex *mtx)
{
	rt_assert(*mtx, "can't unlock unlocked mutex");
	*mtx = 0;
}

#endif
