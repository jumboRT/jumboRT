#include "parser.h"

#include <math.h>

void
	rt_exec_camera(t_world *world, t_parse_ctx *ctx)
{
	t_cam_params	p;

	p.org = rt_vec(ctx);
	p.dir = rt_vec_norm(ctx);
	p.fov = rt_float_range(ctx, 1.0, 179.0);
	p.focus = 1.0f;
	p.blur = 0.0f;
	camera_set(world, &world->camera, p);
	ctx->cam_set = 1;
}

void
	rt_exec_cam_focus(t_world *world, t_parse_ctx *ctx)
{
	t_cam_params	p;

	if (ctx->cam_set == 0)
		rt_parse_error(ctx, "tried to set camera focus before camera was set");
	p.org = world->camera.org;
	p.dir = world->camera.dir;
	p.fov = world->camera.fov;
	p.focus = rt_float_range(ctx, RT_TINY_VAL, RT_HUGE_VAL);
	p.blur = world->camera.blur;
	camera_set(world, &world->camera, p);
}

void
	rt_exec_cam_blur(t_world *world, t_parse_ctx *ctx)
{
	t_cam_params	p;

	if (ctx->cam_set == 0)
		rt_parse_error(ctx, "tried to set camera blur before camera was set");
	p.org = world->camera.org;
	p.dir = world->camera.dir;
	p.fov = world->camera.fov;
	p.focus = world->camera.focus;
	p.blur = rt_float_range(ctx, RT_TINY_VAL, RT_HUGE_VAL);
	camera_set(world, &world->camera, p);
}
