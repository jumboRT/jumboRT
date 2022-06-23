#include "parser.h"

#include "world_impl.h"

void
	rt_exec_sphere(t_world *world, t_parse_ctx *ctx)
{
	t_shape_sphere	sphere;

	sphere.base.shape_type = RT_SHAPE_SPHERE;
	sphere.base.mat_index = 0;
	sphere.pos = rt_vec(ctx);
	sphere.radius = rt_float(ctx) / 2;
	rt_color(ctx);
	world_add_primitive(world, &sphere, sizeof(sphere));
}

