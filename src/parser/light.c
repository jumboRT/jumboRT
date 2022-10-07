#include "parser.h"

#include "mat.h"
#include "shape.h"
#include "world_impl.h"

void
	rt_exec_light(t_world *world, t_parse_ctx *ctx)
{
	t_shape_point	sphere;
	t_material		material;

	sphere.base.data = RT_SHAPE_POINT;
	sphere.pos = rt_vec(ctx);
	material_init(&material, world);
	material.brightness = rt_float_range(ctx, 0.0, 1.0) * 256;
	material.emission = rt_filter(world, ctx);
	material.flags |= RT_MAT_EMITTER;
	sphere.base.data |= world_add_material(world, &material) << 8;
	world_add_primitive(world, &sphere, sizeof(sphere));
}
