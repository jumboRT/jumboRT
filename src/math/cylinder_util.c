/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   cylinder_util.c                                #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:22 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:22 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "rtmath.h"

t_cylinder
	cylinder(t_vec pos, t_vec dir, float height, float radius)
{
	t_cylinder	result;

	result.pos = pos;
	result.dir = dir;
	result.height = height;
	result.radius = radius;
	return (result);
}

t_vec2
	cylinder_uv_mantle(t_cylinder cylinder, t_vec rel_point)
{
	float	u;
	float	v;

	u = rt_atan2(vec_dot(rel_point, vec_x(1.0)),
			vec_dot(rel_point, vec_y(1.0))) / RT_2PI;
	v = (vec_dot(rel_point, vec_z(1.0)) / cylinder.height) + 0.0;
	return (vec2(u, (v / 2.0) + 0.5));
}

t_vec2
	cylinder_uv_cap(t_cylinder cylinder, t_vec rel_point)
{
	t_vec2	uv;

	uv = vec2(x(rel_point), y(rel_point));
	uv = vec2_scale(uv, 1.0 / (4.0 * cylinder.radius));
	uv = vec2_add(uv, vec2(0.5, 0.5));
	uv = vec2_add(uv,
			vec2(0.5 * (z(rel_point) > (cylinder.height / 2.0)), 0.0));
	return (uv);
}

int
	between(float a, float b, float c)
{
	if (a < b && a > c)
		return (1);
	if (a > b && a < c)
		return (1);
	return (0);
}

void
	cylinder_hit_info(t_ray ray, t_cylinder cylinder, t_hit *hit)
{
	(void) ray;
	(void) cylinder;
	(void) hit;
}
