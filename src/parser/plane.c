#include "parser.h"

#include "world_impl.h"

void
	rt_exec_plane(t_world *world, t_parse_ctx *ctx)
{
	t_shape_plane	shape;

	shape.base.data = RT_SHAPE_PLANE;
	shape.plane.pos = rt_vec(ctx);
	shape.plane.normal = rt_vec_norm(ctx);
	rt_material(ctx, world, &shape.base);
	world_add_primitive(world, &shape, sizeof(shape));
}
