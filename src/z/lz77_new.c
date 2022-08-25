#include "z.h"
#include "vector.h"
#include "util.h"
#include "perf.h"

// 1. chains genereren voor herhalende data
// 2. in ztoken_generate gaan er dingen fout als de offset standaard op ZHASH_SIZE staat
// 3. er kunnen erg veel sanity checks weg voor performance, want het is veel te traag
// 4. 

static uint32_t
	lz_hash(uint64_t data)
{
	return (data & ZHASH_MASK);
}

static void
	zring_create(t_zring *ring, size_t n)
{
	ring->data = rt_malloc(sizeof(*ring->data) * n);
	ring->size = n;
	ring->index = 0;
	ring->int_size = 0;
}

static void
	zring_destroy(t_zring *ring)
{
	rt_free(ring->data);
	ring->size = 0;
	ring->index = 0;
	ring->int_size = 0;
}

static t_zchain
	*zring_at(t_zring *ring, int16_t index)
{
	if (index == ZEMPTY)
		return (NULL);
	//rt_assert(index >= 0, "out of bounds access in zring_at");
	//rt_assert((uint16_t) index < ring->size, "out of bounds acces in zring_at");
	return (&ring->data[index]);
}

static int
	zring_isfull(t_zring *ring)
{
	return (ring->int_size == ring->size);
}

static void
	zring_advance(t_zring *ring)
{
	ring->index = (ring->index + 1) % ring->size;
	if (ring->int_size != ring->size)
		ring->int_size++;
}

static void
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

static void
	ztable_destroy(t_ztable *table)
{
	rt_free(table->data);
	table->size = 0;
}

static int16_t
	ztable_at(t_ztable *table, uint32_t hash)
{
	//rt_assert(hash < table->size, "out of bounds access in ztable_at");
	return (table->data[hash].first);
}

static t_zchain
	*zchain_next(t_zring *ring, t_zchain *current)
{
	if (current->next == -1)
		return (NULL);
	return (zring_at(ring, current->next));
}

static uint64_t
	lz_peek_next(const unsigned char *src, size_t offset, size_t src_size)
{
	uint64_t	result;
	size_t		extra;

	//rt_assert(offset < src_size, "read on empty buffer");
	rt_assert(src_size - offset >= ZPEEK_SIZE, "cannot peek past end of buffer");
	// TODO: remove some checks if this assert never triggers ^^
	extra = 0;
	result = 0;
	// TODO: integer overflow check can be removed
	while (extra < ZPEEK_SIZE && offset + extra >= offset
			&& offset + extra < src_size)
	{
		result |= src[offset + extra] << (extra * 8);
		extra++;
	}
	return (result);
}

// TODO: token is never used in chain
static void
	lz_push(t_zstate *state, uint32_t hash, size_t offset)
{
	t_zchain	*link;
	uint32_t	old_hash;

	link = zring_at(&state->ring, state->ring.index);
	if (zring_isfull(&state->ring))
	{
		old_hash = lz_hash(lz_peek_next(state->src, link->offset,
					state->src_size));
		state->table.data[old_hash].first = link->next;
	}
	link->offset = offset;
	link->next = ZEMPTY;
	if (ztable_at(&state->table, hash) == ZEMPTY)
		state->table.data[hash].first = state->ring.index;
	else
		zring_at(&state->ring, state->table.data[hash].last)->next = state->ring.index;
	state->table.data[hash].last = state->ring.index;
	zring_advance(&state->ring);
}

static t_ztoken
	ztoken_at(t_zstate *state, size_t offset)
{
	t_ztoken result;

	result.length = 1;
	result.data.distance = 0;
	result.data.character = state->src[offset];
	return (result);
}

static t_ztoken
	ztoken_generate(const unsigned char *src, size_t offset_a,
			size_t offset_b, size_t src_size)
{
	const unsigned char	*a;
	const unsigned char *end_a;
	const unsigned char	*b;
	t_ztoken			result;
	size_t				max_offset;

	max_offset = src_size - offset_a;
	if (ZTOKEN_MAX_LENGTH < max_offset)
		max_offset = ZTOKEN_MAX_LENGTH;
	a = src + offset_a;
	b = src + offset_b;
	end_a = a + max_offset;
	a += ZHASH_SIZE;
	b += ZHASH_SIZE;
	while (a != end_a && *a == *b)
	{
		a += 1;
		b += 1;
	}
	result.length = a - (src + offset_a);
	result.data.distance = offset_a - offset_b;
	return (result);
}

static int
	ztoken_cmp(t_ztoken a, t_ztoken b)
{
	if (a.length < ZTOKEN_MIN_LENGTH)
		return (-1);
	if (a.data.distance > ZWINDOW_SIZE)
		return (-1);
	if (a.length < b.length)
		return (-1);
	if (a.length > b.length)
		return (1);
	/* TODO check if greater than might be better for our data */
	if (a.data.distance < b.data.distance)
		return (1);
	return (-1);
}

static t_ztoken
	lz_deflate(t_zstate *state, uint32_t hash)
{
	t_zchain	*chain;
	t_ztoken	current;
	t_ztoken	best;

	best = ztoken_at(state, state->offset);
	chain = zring_at(&state->ring, ztable_at(&state->table, hash));
	while (chain != NULL)
	{
		current = ztoken_generate(state->src, state->offset, chain->offset,
				state->src_size);
		if (ztoken_cmp(current, best) > 0)
			best = current;
		chain = zchain_next(&state->ring, chain);
	}
	return (best);
}

static t_ztoken 
	lz_encode(t_zstate *state, uint32_t hash, t_vector *out)
{
	t_ztoken	token;
	size_t		length;
	size_t		offset;

	token = lz_deflate(state, hash);
	vector_push(out, &token);
	offset = 0;
	length = token.length;
	while (offset < length && state->offset + offset + ZHASH_SIZE - 1 < state->src_size)
	{
		hash = lz_hash(lz_peek_next(state->src, state->offset + offset,
					state->src_size));
		lz_push(state, hash, state->offset + offset);
		offset += 1;
	}
	return (token);
}

static void
	lz_advance(t_zstate *state, size_t amount)
{
	//rt_assert(state->offset + amount >= state->offset, "src buffer overflow");
	//rt_assert(state->offset + amount <= state->src_size, "src buffer overflow");
	state->offset += amount;
}

static t_vector 
	lz77_deflate_int(t_zstate *state)
{
	uint64_t	next;
	uint32_t	hash;
	t_ztoken	encoded;
	t_vector	result;

	vector_create(&result, sizeof(t_ztoken), 0);
	while (state->offset + ZHASH_SIZE - 1 < state->src_size)
	{
		next = lz_peek_next(state->src, state->offset, state->src_size);
		hash = lz_hash(next);
		encoded = lz_encode(state, hash, &result);
		lz_advance(state, encoded.length);
	}
	while (state->offset < state->src_size)
	{
		encoded = ztoken_at(state, state->offset);
		vector_push(&result, &encoded);
		lz_advance(state, encoded.length);
	}
	return (result);
}

static void
	lz77_init(t_zstate *state, const void *src, size_t src_size)
{
	state->src = src;
	state->src_size = src_size;
	state->offset = 0;
	ztable_create(&state->table, ZTABLE_SIZE);
	zring_create(&state->ring, ZWINDOW_SIZE);
}

static void
	lz77_destroy(t_zstate *state)
{
	ztable_destroy(&state->table);
	zring_destroy(&state->ring);
}

t_ztoken
	*lz77_deflate(const void *src, size_t src_size, size_t *out_count)
{
	t_zstate	state;
	t_vector	result;

	lz77_init(&state, src, src_size);
	result = lz77_deflate_int(&state);
	if (out_count != NULL)
		*out_count = view_size(result.view);
	lz77_destroy(&state);
	return (result.view.data);
}
