#include "parser.h"

#include "world_impl.h"
#include "util.h"
#include <libft.h>

/* TODO: check indices range */
void
	rt_exec_triangle(t_world *world, t_parse_ctx *ctx)
{
	t_shape_triangle	triangle;

	triangle.base.data = RT_SHAPE_TRIANGLE;
	triangle.a = rt_uint(ctx);
	triangle.b = rt_uint(ctx);
	triangle.c = rt_uint(ctx);
	rt_material(ctx, world, &triangle.base);
	world_add_primitive(world, &triangle, sizeof(triangle));
}

