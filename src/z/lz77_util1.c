#include "z.h"

uint32_t
	lz_hash(uint64_t data)
{
	return (data & ZHASH_MASK);
}

uint64_t
	lz_peek_next(const unsigned char *src, size_t offset, size_t src_size)
{
	uint64_t	result;
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
