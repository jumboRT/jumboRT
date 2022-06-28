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
	t_vec	dir;
	FLOAT	half;
	FLOAT	aspect;

	world->camera.org = rt_vec(ctx);
	dir = rt_vec_norm(ctx);
	half = tan(rt_float(ctx) / 360 * RT_PI);
	aspect = (FLOAT) world->img_meta.width / world->img_meta.height;
	world->camera.u = vec_norm(vec_cross(dir, vec(0, 0, 1)));
	world->camera.v = vec_norm(vec_cross(dir, world->camera.u));
	world->camera.u = vec_scale(world->camera.u, half * -aspect);
	world->camera.v = vec_scale(world->camera.v, half);
	world->camera.base = vec_sub(dir, vec_add(world->camera.u, world->camera.v));
	world->camera.u = vec_scale(world->camera.u, 2.0 / world->img_meta.width);
	world->camera.v = vec_scale(world->camera.v, 2.0 / world->img_meta.height);
}

