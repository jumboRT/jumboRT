#include "main.h"
#include "state.h"
#include "work.h"
#include "keycode.h"

#include <stdio.h>

#ifndef RT_JOINC

# define FLY_SPEED 1

static void
	rt_key_render_mode(int keycode, t_work *work)
{
	int	new_mode;

	if (keycode == RT_KEY_1)
		new_mode = RT_RENDER_MODE_DEFAULT;
	else if (keycode == RT_KEY_2)
		new_mode = RT_RENDER_MODE_UV;
	else if (keycode == RT_KEY_3)
		new_mode = RT_RENDER_MODE_GEOMETRIC_NORMAL;
	else if (keycode == RT_KEY_4)
		new_mode = RT_RENDER_MODE_SHADING_NORMAL;
	else if (keycode == RT_KEY_5)
		new_mode = RT_RENDER_MODE_ALBEDO;
	else
		return ;
	rt_work_lock(work);
	work->state->world->render_mode = new_mode;
	rt_work_unlock(work);
}

static void
	rt_key_camera_int(int key, t_work *work, t_cam_params p)
{
	float	s;
	t_vec	left;

	s = 1.0f;
	left = vec_norm(vec_cross(p.dir, vec_z(1.0)));
	if (key == RT_KEY_LEFT || key == RT_KEY_DOWN || key == RT_KEY_S
		|| key == RT_KEY_D || key == RT_KEY_SHIFT || key == RT_KEY_Z)
		s = -1.0f;
	if (key == RT_KEY_LEFT || key == RT_KEY_RIGHT)
		p.dir = vec_rotate(vec_z(1.0), p.dir, s * RT_PI / (12 * 90 / p.fov));
	else if (key == RT_KEY_UP || key == RT_KEY_DOWN)
		p.dir = vec_rotate(left, p.dir, s * RT_PI / (12 * 90 / p.fov));
	else if (key == RT_KEY_S || key == RT_KEY_W)
		p.org = vec_add(p.org, vec_scale(p.dir, FLY_SPEED * s));
	else if (key == RT_KEY_A || key == RT_KEY_D)
		p.org = vec_add(p.org, vec_scale(left, FLY_SPEED * s));
	else if (key == RT_KEY_SPACE || key == RT_KEY_SHIFT)
		p.org = vec_add(p.org, vec_z(FLY_SPEED * s));
	else if (key == RT_KEY_Z || key == RT_KEY_X)
		p.fov = rt_max(5.0, p.fov + 5 * s);
	else
		return ;
	rt_work_lock(work);
	camera_set(work->state->world, &work->state->world->camera, p);
	rt_work_unlock(work);
}

static t_cam_params
	rt_key_cam_params(t_work *work)
{
	t_camera		*camera;
	t_cam_params	p;

	camera = &work->state->world->camera;
	p.org = camera->org;
	p.dir = camera->dir;
	p.fov = camera->fov;
	p.focus = camera->focus;
	p.blur = camera->blur;
	return (p);
}

int
	rt_key_down(int keycode, void *ctx)
{
	t_work			*work;
	t_cam_params	p;

	work = ctx;
	p = rt_key_cam_params(work);
	if (keycode == RT_KEY_ESC || keycode == RT_KEY_Q)
		rt_exit(work);
	else if (keycode == RT_KEY_R)
	{
		rt_work_lock(work);
		rt_work_unlock(work);
	}
	else if (keycode == RT_KEY_I)
	{
		printf("cam org %f %f %f\n", x(p.org), y(p.org), z(p.org));
		printf("cam dir %f %f %f\n", x(p.dir), y(p.dir), z(p.dir));
	}
	rt_key_camera_int(keycode, work, p);
	rt_key_render_mode(keycode, work);
	return (0);
}

#endif
