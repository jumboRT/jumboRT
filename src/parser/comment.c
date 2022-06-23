#include "parser.h"

void
	rt_exec_comment(t_world *world, t_parse_ctx *ctx)
{
	(void) world;
	while (*ctx->data != '\n' && *ctx->data != '\0')
		rt_advance(ctx);
}
