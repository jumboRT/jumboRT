#include "z.h"

static void
	lz_deflate_loop1(t_zlzctx *ctx)
{
	if (ctx->prev_length == ctx->current_length)
	{
		ctx->current_length = ztoken_mismatch(ctx->state, ctx->state->offset,
				ctx->chain->offset, ctx->current_length);
		if (ctx->current_length >= ctx->best.length)
		{
			if (ctx->chain_ref != NULL)
			{
				ctx->chain_ref->next_best = ctx->chain - ctx->state->ring.data;
				ctx->chain_ref->next_best_length = ctx->best.length;
			}
			ctx->chain_ref = ctx->chain;
			if (ctx->current_length >= ZTOKEN_MIN_LENGTH)
			{
				ctx->best.length = ctx->current_length;
				ctx->best.data.distance = ctx->state->offset
					- ctx->chain->offset;
			}
		}
	}
	else if (ctx->prev_length < ctx->current_length)
		ctx->current_length = ctx->prev_length;
}

static void
	lz_deflate_loop2(t_zlzctx *ctx)
{
	if (ctx->chain->next_best != ZEMPTY
		&& ctx->current_length == ctx->chain->next_best_length)
	{
		ctx->prev_length = ctx->chain->next_best_length;
		ctx->chain = zring_at(&ctx->state->ring, ctx->chain->next_best);
	}
	else
	{
		ctx->prev_length = ctx->chain->next_length;
		ctx->chain = zring_at(&ctx->state->ring, ctx->chain->next);
	}
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

t_ztoken
	lz_deflate(t_zstate *state, uint32_t hash)
{
	t_zlzctx	ctx;

	ctx.state = state;
	ctx.hash = hash;
	ctx.best = ztoken_at(ctx.state, ctx.state->offset);
	ctx.chain = zring_at(&ctx.state->ring, ztable_at(&ctx.state->table,
				ctx.hash));
	ctx.chain_ref = NULL;
	ctx.current_length = ZHASH_SIZE;
	ctx.prev_length = ZHASH_SIZE;
	while (ctx.chain != NULL)
	{
		lz_deflate_loop1(&ctx);
		lz_deflate_loop2(&ctx);
	}
	return (ctx.best);
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
