#include "parser.h"

#include "shape.h"
#include "world_impl.h"

void
	rt_exec_paraboloid(t_world *world, t_parse_ctx *ctx)
{
	t_shape_paraboloid	paraboloid;

	paraboloid.base.data = RT_SHAPE_PARABOLOID;
	paraboloid.paraboloid.pos = rt_vec(ctx);
	paraboloid.paraboloid.a = rt_float(ctx);
	paraboloid.paraboloid.b = rt_float(ctx);
	rt_material(ctx, world, &paraboloid.base);
	world_add_primitive(world, &paraboloid, sizeof(paraboloid));
}

void
	rt_exec_hyperboloid(t_world *world, t_parse_ctx *ctx)
{
	t_shape_hyperboloid	hyperboloid;

	hyperboloid.base.data = RT_SHAPE_HYPERBOLOID;
	hyperboloid.hyperboloid.pos = rt_vec(ctx);
	hyperboloid.hyperboloid.a = rt_float(ctx);
	hyperboloid.hyperboloid.b = rt_float(ctx);
	hyperboloid.hyperboloid.c = rt_float(ctx);
	rt_material(ctx, world, &hyperboloid.base);
	world_add_primitive(world, &hyperboloid, sizeof(hyperboloid));
}

