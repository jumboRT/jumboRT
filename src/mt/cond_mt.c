/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   cond_mt.c                                      #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:19 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:19 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "mt.h"
#include "util.h"

#ifdef RT_MT

void
	cond_init(t_cond *cnd)
{
	int	ret;

	ret = pthread_cond_init(cnd, NULL);
	rt_assert(!ret, "cond init failed");
}

void
	cond_destroy(t_cond *cnd)
{
	int	ret;

	ret = pthread_cond_destroy(cnd);
	rt_assert(!ret, "cond destroy failed");
}

void
	cond_wait(t_cond *cnd, t_mutex *mtx)
{
	int	ret;

	ret = pthread_cond_wait(cnd, mtx);
	rt_assert(!ret, "cond wait failed");
}

void
	cond_broadcast(t_cond *cnd)
{
	int	ret;

	ret = pthread_cond_broadcast(cnd);
	rt_assert(!ret, "cond wait failed");
}

#endif
