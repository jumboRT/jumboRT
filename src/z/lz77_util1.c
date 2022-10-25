/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   lz77_util1.c                                   #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:13 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:13 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "z.h"

t_uint32
	lz_hash(t_uint64 data)
{
	return (data & ZHASH_MASK);
}

t_uint64
	lz_peek_next(const unsigned char *src, size_t offset, size_t src_size)
{
	t_uint64	result;
	size_t		extra;

	extra = 0;
	result = 0;
	(void) src_size;
	while (extra < ZHASH_SIZE)
	{
		result |= src[offset + extra] << (extra * 8);
		extra++;
	}
	return (result);
}

void
	lz_advance(t_zstate *state, size_t amount)
{
	state->offset += amount;
}

void
	lz77_init(t_zstate *state, const void *src, size_t src_size)
{
	state->src = src;
	state->src_size = src_size;
	state->offset = 0;
	ztable_create(&state->table, ZTABLE_SIZE);
	zring_create(&state->ring, ZWINDOW_SIZE);
}

void
	lz77_destroy(t_zstate *state)
{
	ztable_destroy(&state->table);
	zring_destroy(&state->ring);
}
