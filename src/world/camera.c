/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   camera.c                                       #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:28 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:28 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "world.h"

#include <math.h>

void
	camera_set(const t_world *world, t_camera *camera, t_cam_params p)
{
	float	half;
	float	aspect;

	camera->dir = p.dir;
	camera->org = p.org;
	camera->fov = p.fov;
	half = tan(p.fov / 360 * RT_PI);
	aspect = (float) world->img_meta.width / world->img_meta.height;
	camera->u_norm = vec_norm(vec_cross(p.dir, vec(0, 0, 1, 0)));
	camera->v_norm = vec_norm(vec_cross(p.dir, camera->u_norm));
	camera->u = vec_scale(camera->u_norm, -half);
	camera->v = vec_scale(camera->v_norm, half / aspect);
	camera->base = vec_scale(vec_sub(p.dir,
				vec_add(camera->u, camera->v)), p.focus);
	camera->u = vec_scale(camera->u, 2.0 / world->img_meta.width * p.focus);
	camera->v = vec_scale(camera->v, 2.0 / world->img_meta.height * p.focus);
	camera->focus = p.focus;
	camera->blur = p.blur;
}
