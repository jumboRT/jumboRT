#include "rt.h"
#include <stdlib.h>
#include <ft_printf.h>

int
	rt_exit(void *ctx)
{
	thread_stop(ctx);
	exit(EXIT_SUCCESS);
	return (0);
}

int
	rt_key_down(int key, void *ctx)
{
	t_rt_state	*state;
	t_camera	*cam;
	FLOAT		yaw;
	FLOAT		pitch;

	state = ctx;
	cam = state->scene.camera;
	project_angles(cam->dir, &yaw, &pitch);
	ft_printf("key %d\n", key);
	if (key == RT_KEY_ESC || key == RT_KEY_Q)
		rt_exit(ctx);
	mutex_lock(&state->mtx);
	if (key == RT_KEY_R)
	{
		thread_reset(ctx);
	}
	if (key == RT_KEY_P)
	{
		state->use_conic = !state->use_conic;
		thread_reset(ctx);
	}
	if (key == RT_KEY_W)
	{
		cam->pos = vec_add(cam->pos, cam->dir);
		thread_reset(ctx);
	}
	if (key == RT_KEY_S)
	{
		cam->pos = vec_sub(cam->pos, cam->dir);
		thread_reset(ctx);
	}
	if (key == RT_KEY_LEFT)
	{
		yaw += RT_PI / 16;
		cam->dir = project_rotate(vec(0, 1, 0, 0), yaw, pitch);
		thread_reset(ctx);
	}
	if (key == RT_KEY_RIGHT)
	{
		yaw -= RT_PI / 16;
		cam->dir = project_rotate(vec(0, 1, 0, 0), yaw, pitch);
		thread_reset(ctx);
	}
	if (key == RT_KEY_UP)
	{
		pitch += RT_PI / 16;
		cam->dir = project_rotate(vec(0, 1, 0, 0), yaw, pitch);
		thread_reset(ctx);
	}
	if (key == RT_KEY_DOWN)
	{
		pitch -= RT_PI / 16;
		cam->dir = project_rotate(vec(0, 1, 0, 0), yaw, pitch);
		thread_reset(ctx);
	}
	mutex_unlock(&state->mtx);
	return (0);
}

