#include "parser.h"

#include "world_impl.h"
#include "util.h"
#include <libft.h>

void
	rt_exec_triangle(t_world *world, t_parse_ctx *ctx)
{
	t_shape_triangle	triangle;

	triangle.base.data = RT_SHAPE_TRIANGLE;
	triangle.a = rt_uint(ctx);
	if (triangle.a >= ctx->max_index) {
		rt_parse_error(ctx,
				"%u exceeds or is equal to the max index value %d",
				triangle.a, (unsigned int) ctx->max_index); 
	}
	triangle.b = rt_uint(ctx);
	if (triangle.b >= ctx->max_index) {
		rt_parse_error(ctx,
				"%u exceeds or is equal to the max index value %d",
				triangle.b, (unsigned int) ctx->max_index); 
	}
	triangle.c = rt_uint(ctx);
	if (triangle.c >= ctx->max_index) {
		rt_parse_error(ctx,
				"%u exceeds or is equal to the max index value %d",
				triangle.c, (unsigned int) ctx->max_index); 
	}
	rt_material(ctx, world, &triangle.base);
	world_add_primitive(world, &triangle, sizeof(triangle));
}

