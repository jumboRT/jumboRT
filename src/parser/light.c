#include "parser.h"

#include "world_impl.h"

void
	rt_exec_light(t_world *world, t_parse_ctx *ctx)
{
	t_shape_sphere	sphere;
	t_material		material;

	sphere.base.data = RT_SHAPE_SPHERE;
	sphere.pos = rt_vec(ctx);
	sphere.radius = 1.0;
	material_init(&material, world);
	material.brightness = rt_float_range(ctx, 0.0, 1.0) * 256;
	material.emission = rt_filter(world, ctx);
	material.flags |= RT_MAT_EMITTER;
	sphere.base.data |= world_add_material(world, &material) << 8;
	world_add_primitive(world, &sphere, sizeof(sphere));
}
