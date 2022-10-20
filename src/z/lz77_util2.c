#include "z.h"
#include "vector.h"
#include "util.h"
#include "perf.h"

size_t
	ztoken_mismatch(t_zstate *state, size_t offset_a,
			size_t offset_b, size_t min)
{
	const unsigned char	*a;
	const unsigned char	*end_a;
	const unsigned char	*b;
	size_t				max_offset;

	max_offset = state->src_size - offset_a;
	if (ZTOKEN_MAX_LENGTH < max_offset)
		max_offset = ZTOKEN_MAX_LENGTH;
	a = state->src + offset_a;
	b = state->src + offset_b;
	end_a = a + max_offset;
	a += min;
	b += min;
	while (a + 7 < end_a && *(uint64_t *) a == *(uint64_t *) b)
	{
		a += 8;
		b += 8;
	}
	while (a + 0 < end_a && *(uint8_t *) a == *(uint8_t *) b)
	{
		a += 1;
		b += 1;
	}
	return (a - (state->src + offset_a));
}

static void
	lz_push(t_zstate *state, uint32_t hash, size_t offset)
{
	t_zchain	*link;
	t_zchain	*last;

	link = zring_at(&state->ring, state->ring.index);
	if (zring_isfull(&state->ring))
		state->table.data[link->hash].first = link->next;
	link->offset = offset;
	link->hash = hash;
	link->next = ZEMPTY;
	link->next_length = 0;
	link->next_best = ZEMPTY;
	link->next_best_length = 0;
	if (ztable_at(&state->table, hash) == ZEMPTY)
		state->table.data[hash].first = state->ring.index;
	else
	{
		last = zring_at(&state->ring, state->table.data[hash].last);
		last->next = state->ring.index;
		last->next_length
			= ztoken_mismatch(state, offset, last->offset, ZHASH_SIZE);
	}
	state->table.data[hash].last = state->ring.index;
	zring_advance(&state->ring);
}

t_ztoken
	ztoken_at(t_zstate *state, size_t offset)
{
	t_ztoken	result;

	result.length = 1;
	result.data.distance = 0;
	result.data.character = state->src[offset];
	return (result);
}

t_ztoken
	lz_encode(t_zstate *state, uint32_t hash, t_vector *out)
{
	t_ztoken	token;
	size_t		length;
	size_t		offset;

	token = lz_deflate(state, hash);
	vector_push(out, &token);
	offset = 0;
	length = token.length;
	while (offset < length && state->offset + offset + ZHASH_SIZE - 1
		< state->src_size)
	{
		hash = lz_hash(lz_peek_next(state->src, state->offset + offset,
					state->src_size));
		lz_push(state, hash, state->offset + offset);
		offset += 1;
	}
	return (token);
}
