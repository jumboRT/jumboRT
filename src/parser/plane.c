#include "parser.h"

#include "world_impl.h"

void
	rt_exec_plane(t_world *world, t_parse_ctx *ctx)
{
	t_shape_plane	shape;

	shape.base.shape_type = RT_SHAPE_PLANE;
	shape.base.mat_index = 0;
	shape.plane.pos = rt_vec(ctx);
	shape.plane.normal = rt_vec_norm(ctx);
	rt_color(ctx);
	world_add_primitive(world, &shape, sizeof(shape));
}
