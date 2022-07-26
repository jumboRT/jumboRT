#include "parser.h"

#include <math.h>

/* TODO: check for multiple cameras */
/* TODO: check FOV in range */
void
	rt_exec_camera(t_world *world, t_parse_ctx *ctx)
{
	t_vec pos;
	t_vec dir;
	FLOAT fov;

	pos = rt_vec(ctx);
	dir = rt_vec_norm(ctx);
	fov = rt_float(ctx);
	camera_set(world, &world->camera, pos, dir, fov);
}

