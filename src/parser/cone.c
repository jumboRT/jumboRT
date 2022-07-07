#include "parser.h"

#include "world_impl.h"

void
	rt_exec_cone(t_world *world, t_parse_ctx *ctx)
{
	t_shape_cone	shape;

	shape.base.shape_type = RT_SHAPE_CONE;
	shape.base.mat_index = 0;
	shape.cone.pos = rt_vec(ctx);
	shape.cone.dir = rt_vec_norm(ctx);
	shape.cone.angle = rt_float(ctx);
	shape.cone.height = rt_float(ctx);
	rt_color(ctx);
	world_add_primitive(world, &shape, sizeof(shape));
}
