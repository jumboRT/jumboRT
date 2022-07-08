#include "work.h"

/* TODO: ensure ctx->seed doesn't get set to 0 */
void
	ctx_init(t_context *ctx)
{
	static t_seed	seed = 7549087012;

	ctx->seed = rt_random(&seed);
}

void
	ctx_destroy(t_context *ctx)
{
	(void) ctx;
}

