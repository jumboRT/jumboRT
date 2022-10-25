/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   zbuf.c                                         #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:14 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:14 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "z.h"

#include "util.h"

void
	zbuf_create(t_zbuf *zb, void *data, size_t size)
{
	zb->data = data;
	zb->index = 0;
	zb->size = size;
	zb->capacity = size;
	zb->bit = 0;
}

void
	zbuf_next(t_zbuf *zb)
{
	if (zb->bit != 0)
	{
		zb->bit = 0;
		zb->index += 1;
	}
}
