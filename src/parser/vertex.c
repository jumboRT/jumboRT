#include "parser.h"

#include "world_impl.h"

void
	rt_exec_vertex(t_world *world, t_parse_ctx *ctx)
{
	t_vertex	vertex;

	vertex.pos = rt_vec(ctx);
	world_add_vertex(world, &vertex);
}

