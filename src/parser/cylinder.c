#include "parser.h"

#include "world_impl.h"

void
	rt_exec_cylinder(t_world *world, t_parse_ctx *ctx)
{
	t_shape_cylinder	shape;

	shape.base.shape_type = RT_SHAPE_CYLINDER;
	shape.base.mat_index = 0;
	shape.cylinder.pos = rt_vec(ctx);
	shape.cylinder.dir = rt_vec_norm(ctx);
	shape.cylinder.radius = rt_float(ctx) / 2;
	shape.cylinder.height = rt_float(ctx);
	rt_color(ctx);
	world_add_primitive(world, &shape, sizeof(shape));
}
