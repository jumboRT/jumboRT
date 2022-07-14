#include "parser.h"

#include <math.h>

void
	init_camera(t_world *world)
{
	FLOAT	aspect;

	aspect = (FLOAT) world->img_meta.width / world->img_meta.height;
	world->camera.org = vec(0, 0, 0);
	world->camera.base = vec(1, -1 * aspect, 1);
	world->camera.u = vec(0, 2.0 / world->img_meta.height, 0);
	world->camera.v = vec(0, 0, -2.0 / world->img_meta.height);
}

/* TODO: check for multiple cameras */
/* TODO: check FOV in range */
void
	rt_exec_camera(t_world *world, t_parse_ctx *ctx)
{
	camera_set(world, &world->camera, rt_vec(ctx), rt_vec_norm(ctx), rt_float(ctx));
}

