#include "parser.h"

#include <math.h>

void
	rt_exec_camera(t_world *world, t_parse_ctx *ctx)
{
	t_vec pos;
	t_vec dir;
	FLOAT fov;

	pos = rt_vec(ctx);
	dir = rt_vec_norm(ctx);
	fov = rt_float_range(ctx, 1.0, 179.0);
	camera_set(world, &world->camera, pos, dir, fov);
}

