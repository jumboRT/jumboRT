#include "parser.h"

#include "world_impl.h"

/* TODO: check indices range */
void
	rt_exec_triangle(t_world *world, t_parse_ctx *ctx)
{
	t_shape_triangle	triangle;

	triangle.base.shape_type = RT_SHAPE_TRIANGLE;
	triangle.base.mat_index = 0;
	triangle.a = rt_uint(ctx);
	triangle.b = rt_uint(ctx);
	triangle.c = rt_uint(ctx);
	world_add_primitive(world, &triangle, sizeof(triangle));
}

