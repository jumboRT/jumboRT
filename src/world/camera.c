#include "world.h"

#include <math.h>

void
    camera_set(const t_world *world, t_camera *camera, t_vec org, t_vec dir, float fov, float focus, float blur)
{

	float	half;
	float	aspect;

	camera->dir = dir;
	camera->org = org;
	camera->fov = fov;
	half = tan(fov / 360 * RT_PI);
	aspect = (float) world->img_meta.width / world->img_meta.height;
	camera->u_norm = vec_norm(vec_cross(dir, vec(0, 0, 1, 0)));
	camera->v_norm = vec_norm(vec_cross(dir, camera->u_norm));
	camera->u = vec_scale(camera->u_norm, -half);
	camera->v = vec_scale(camera->v_norm, half / aspect);
	camera->base = vec_scale(vec_sub(dir, vec_add(camera->u, camera->v)), focus);
	camera->u = vec_scale(camera->u, 2.0 / world->img_meta.width * focus);
	camera->v = vec_scale(camera->v, 2.0 / world->img_meta.height * focus);
	camera->focus = focus;
	camera->blur = blur;
}
