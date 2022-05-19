#include "rt.h"

#include <math.h>
#include <stdio.h>

t_ray
	projection_ray(t_rt_state *state, int x, int y)
{
	t_camera	*camera;
	FLOAT		yaw;
	FLOAT		pitch;
	t_ray		ray;

	camera = state->scene.camera;
	yaw = atan2(camera->dir.v[Y], camera->dir.v[X]);
	pitch = atan2(camera->dir.v[Z], camera->dir.v[X]
			* sin(yaw) + camera->dir.v[Y] * cos(yaw));
	yaw += ((FLOAT) x / state->img.width - 0.5)
		* camera->fov / 180 * 3.14159;
	pitch += -((FLOAT) y / state->img.height - 0.5)
		* camera->fov / 180 * 3.14159
		/ state->img.width * state->img.height;
	ray.pos = camera->pos;
	ray.dir = vec(cos(pitch) * cos(yaw), cos(pitch) * sin(yaw), sin(pitch), 0);
	return (ray);
}
