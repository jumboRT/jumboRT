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
	if (paraboloid.paraboloid.a == 0)
		rt_parse_error(ctx, "variable a of a paraboloid may not be zero");
	paraboloid.paraboloid.b = rt_float(ctx);
	if (paraboloid.paraboloid.b == 0)
		rt_parse_error(ctx, "variable b of a paraboloid may not be zero");
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
	if (hyperboloid.hyperboloid.a == 0)
		rt_parse_error(ctx, "variable a of a hyperboloid may not be zero");
	hyperboloid.hyperboloid.b = rt_float(ctx);
	if (hyperboloid.hyperboloid.b == 0)
		rt_parse_error(ctx, "variable b of a hyperboloid may not be zero");
	hyperboloid.hyperboloid.c = rt_float(ctx);
	if (hyperboloid.hyperboloid.c == 0)
		rt_parse_error(ctx, "variable c of a hyperboloid may not be zero");
	rt_material(ctx, world, &hyperboloid.base);
	world_add_primitive(world, &hyperboloid, sizeof(hyperboloid));
}

