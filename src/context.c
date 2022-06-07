#include "rt.h"

void
	context_create(t_thread_ctx *ctx, t_rt_state *state)
{
	ctx->seed = 7549087012;
	ctx->visited = rt_malloc(sizeof(*ctx->visited) * state->scene.count);
	ctx->state = state;
}

void
	context_destroy(t_thread_ctx *ctx)
{
	rt_free(ctx->visited);
}
