#include "parser.h"

#include <math.h>

void
	rt_exec_camera(t_world *world, t_parse_ctx *ctx)
{
	t_vec pos;
	t_vec dir;
	float fov;

	pos = rt_vec(ctx);
	dir = rt_vec_norm(ctx);
	fov = rt_float_range(ctx, 1.0, 179.0);
	camera_set(world, &world->camera, pos, dir, fov, 1.0, 0.0);
	ctx->cam_set = 1;
}

void
	rt_exec_cam_focus(t_world *world, t_parse_ctx *ctx)
{
	float	focus;

	if (ctx->cam_set == 0)
		rt_parse_error(ctx, "tried to set camera focus before camera was set");
	focus = rt_float_range(ctx, RT_TINY_VAL, RT_HUGE_VAL);
	camera_set(world, &world->camera, world->camera.org, world->camera.dir,
			world->camera.fov, focus, world->camera.blur);
}

void
	rt_exec_cam_blur(t_world *world, t_parse_ctx *ctx)
{
	float	blur;

	if (ctx->cam_set == 0)
		rt_parse_error(ctx, "tried to set camera blur before camera was set");
	blur = rt_float_range(ctx, RT_TINY_VAL, RT_HUGE_VAL);
	camera_set(world, &world->camera, world->camera.org, world->camera.dir,
			world->camera.fov, world->camera.focus, blur);
}
