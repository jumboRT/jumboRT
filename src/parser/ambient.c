#include "parser.h"

/* TODO: add brightness */
void
	rt_exec_ambient(t_world *world, t_parse_ctx *ctx)
{
	world->ambient_filter = rt_filter(world, ctx);
}
