#include "parser.h"

#include "world_impl.h"

void
	rt_exec_light(t_world *world, t_parse_ctx *ctx)
{
	t_shape_sphere	sphere;
	t_material		material;
	FLOAT			intensity;
	t_vec			color;

	sphere.base.data = RT_SHAPE_SPHERE;
	sphere.pos = rt_vec(ctx);
	sphere.radius = 1.0;
	intensity = rt_float_range(ctx, 0.0, 1.0) * 256.0;
	color = rt_color(ctx);
	material_init(&material);
	material.emission = vec_scale(color, intensity);
	material.albedo = vec(0.0, 0.0, 0.0);
	material.reflective = 0;
	material.refractive = 0;
	sphere.base.data |= world_add_material(world, &material, sizeof(material)) << 8;
	world_add_primitive(world, &sphere, sizeof(sphere));
}
