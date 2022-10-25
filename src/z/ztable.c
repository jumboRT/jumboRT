/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   ztable.c                                       #  # #  #   #  ##   ###   */
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
	ztable_create(t_ztable *table, size_t n)
{
	table->data = rt_malloc(sizeof(*table->data) * n);
	table->size = n;
	while (n--)
	{
		table->data[n].first = ZEMPTY;
		table->data[n].last = ZEMPTY;
	}
}

void
	ztable_destroy(t_ztable *table)
{
	rt_free(table->data);
	table->size = 0;
}

t_int16
	ztable_at(t_ztable *table, t_uint32 hash)
{
	return (table->data[hash].first);
}
