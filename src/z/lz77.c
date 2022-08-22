#include "z.h"
#include "vector.h"
#include "util.h"

//TODO, I know this is not the right place, but somewhere in networking uses
//the wrong malloc

static void
	zchain_destroy_one(t_zchain *chain)
{
	rt_free(chain);
}

static void
	zchain_destroy(t_zchain *chain)
{
	if (chain == NULL)
		return ;
	zchain_destroy(chain->next);
	zchain_destroy_one(chain);
}

static void
	ztable_create(t_ztable *table, size_t size)
{
	table->data = rt_malloc(size * sizeof(*table->data));
	table->size = size;
	while (size--)
		table->data[size] = NULL;
}

static void
	ztable_destroy(t_ztable *table)
{
	size_t	idx;

	idx = 0;
	while (idx < table->size)
	{
		if (table->data[idx] != NULL)
			zchain_destroy(table->data[idx]);
		++idx;
	}
	table->size = 0;
	rt_free(table->data);
}

static uint32_t
	lz_match_count(const unsigned char *a, const unsigned char *b, size_t len)
{
	uint32_t	idx;

	idx = 0;
	while (idx < len && a[idx] == b[idx])
		++idx;
	return (idx);
}

static t_zchain
	*ztable_chain_at(t_ztable *table, uint32_t hash)
{
	rt_assert(hash < table->size, "out of bounds access");
	return (table->data[hash]);
}

static void
	ztable_delete(t_ztable *table, uint32_t hash, t_zchain *link)
{
	t_zchain	*chain;

	chain = ztable_chain_at(table, hash);
	if (chain == link)
	{
		table->data[hash] = chain->next;
		zchain_destroy_one(chain);
		return;
	}
	while (chain->next != NULL && chain->next != link)
		chain = chain->next;
	rt_assert(chain->next != NULL, "could not delete link");
	chain->next = chain->next->next;
}

static void
	zchain_push(t_zchain **start, t_zchain link)
{
	t_zchain	*mlink;
	t_zchain	*tmp;

	mlink = rt_malloc(sizeof(link));
	mlink->token = link.token;
	mlink->offset = link.offset;
	mlink->next = NULL;
	if (*start == NULL)
		*start = mlink;
	else
	{
		tmp = *start;
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = mlink;
	}
}

static void
	ztable_insert(t_ztable *table, uint32_t hash, t_zchain link)
{
	rt_assert(hash < table->size, "out of bounds access");
	zchain_push(&table->data[hash], link);
}

static size_t
	zmin(size_t a, size_t b)
{
	if (a < b)
		return (a);
	return (b);
}
/*
			current_match = lz_match_count(&state->src[offset],
					&state->src[current->offset],
					zmin(state->src_size - offset, &state->src[offset] - state->win));
*/
static t_zchain
	*lz_find(t_zstate *state, uint32_t hash, size_t offset, uint32_t *match_count)
{
	t_zchain	*current;
	t_zchain	*best;
	t_zchain	*tmp;
	uint32_t	current_match;

	current = ztable_chain_at(&state->table, hash);
	best = current;
	*match_count = 0;
	while (current != NULL)
	{
		if (offset - current->offset >= ZWINDOW_SIZE)
		{
			tmp = current->next;
			ztable_delete(&state->table, hash, current);
			current = tmp;
			continue;
		}
		else
		{
			current_match = lz_match_count(&state->src[offset],
					&state->src[current->offset],
					zmin(state->src_size - offset, rt_min(ZWINDOW_SIZE, offset)));
			if (current_match > *match_count)
			{
				*match_count = current_match;
				best = current;
			}
		}
		current = current->next;
	}
	return (best);
}

static t_zchain
	lz_chain_link(size_t abs_offset, uint32_t rel_offset, uint32_t length, unsigned char next)
{
	t_zchain	link;

	link.offset = abs_offset;
	link.token.length = length;
	if (length == 0)
		link.token.data.character = next;
	else
		link.token.data.distance = rel_offset;
	link.next = NULL;
	return (link);
}

static int
	lz_encode(t_zstate *state, uint32_t hash, size_t offset)
{
	uint32_t	match_count;
	t_zchain	link;
	t_zchain	*match;

	match = lz_find(state, hash, offset, &match_count);
	if (match != NULL)
	{
		link = lz_chain_link(offset, offset - match->offset, match_count, ZEMPTY);
	}
	else
	{
		match_count = 1;
		link = lz_chain_link(offset, 0, 0, state->src[offset]);
	}
	ztable_insert(&state->table, hash, link);
	vector_push(&state->result, &link.token);
	return (match_count);
}

static uint32_t
	lz_read_next(t_zstate *state, size_t offset)
{
	uint32_t	result;
	uint32_t	idx;

	result = 0;
	idx = 0;
	while (idx < ZHASH_SIZE && offset + idx < state->src_size)
	{
		rt_assert(idx <= 8, "something went really wrong");
		result |= state->src[offset + idx] << (8 * idx);
		++idx;
	}
	return (result);
}

static void
	lz77_deflate_int(t_zstate *state)
{
	uint32_t			hash;
	uint32_t			len;
	size_t				offset;

	offset = 0;
	while (offset < state->src_size)
	{
		hash = lz_read_next(state, offset);
		len = lz_encode(state, hash, offset);
		offset += len;
		if (offset >= ZWINDOW_SIZE)
			state->win = state->src + (offset - ZWINDOW_SIZE);
	}
}

t_ztoken
	*lz77_deflate(void *src, size_t src_size, size_t *dst_size)
{
	t_zstate	state;

	state.src = src;
	state.win = src;
	state.src_size = src_size;
	ztable_create(&state.table, ZTABLE_SIZE);
	vector_create(&state.result, sizeof(t_ztoken), 0);
	lz77_deflate_int(&state);
	*dst_size = view_size(state.result.view);
	ztable_destroy(&state.table);
	return (state.result.view.data);
}
