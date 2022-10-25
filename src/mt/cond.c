/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   cond.c                                         #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:19 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:19 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "mt.h"

#ifndef RT_MT

void
	cond_init(t_cond *cnd)
{
	(void) cnd;
}

void
	cond_destroy(t_cond *cnd)
{
	(void) cnd;
}

void
	cond_wait(t_cond *cnd, t_mutex *mtx)
{
	(void) cnd;
	(void) mtx;
}

void
	cond_broadcast(t_cond *cnd)
{
	(void) cnd;
}

#endif
