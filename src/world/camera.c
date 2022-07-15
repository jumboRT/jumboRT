#include "world.h"

#include <math.h>

void
    camera_set(const t_world *world, t_camera *camera, t_vec org, t_vec dir, FLOAT fov)
{

	FLOAT	half;
	FLOAT	aspect;

	camera->dir = dir;
	camera->org = org;
	half = tan(fov / 360 * RT_PI);
	aspect = (FLOAT) world->img_meta.width / world->img_meta.height;
	camera->u = vec_norm(vec_cross(dir, vec(0, 0, 1)));
	camera->v = vec_norm(vec_cross(dir, camera->u));
	camera->u = vec_scale(camera->u, -half);
	camera->v = vec_scale(camera->v, half / aspect);
	camera->base = vec_sub(dir, vec_add(camera->u, camera->v));
	camera->u = vec_scale(camera->u, 2.0 / world->img_meta.width);
	camera->v = vec_scale(camera->v, 2.0 / world->img_meta.height);
}
