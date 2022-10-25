/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   plane.c                                        #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:21 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:21 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "rtmath.h"

t_plane
	plane(t_vec pos, t_vec normal)
{
	t_plane	result;

	result.pos = pos;
	result.normal = normal;
	return (result);
}

static t_vec2
	plane_get_uv(t_plane plane, t_vec point, t_vec up, t_vec right)
{
	t_vec	op;

	op = vec_sub(plane.pos, point);
	return (vec_change_basis2(op, up, right));
}

void
	plane_hit_info(t_ray ray, t_plane plane, t_hit *hit)
{
	(void) ray;
	hit->dpdu = vec_tangent(plane.normal);
	hit->dpdv = vec_norm(vec_cross(plane.normal, hit->dpdu));
	hit->uv = plane_get_uv(plane, hit->pos, hit->dpdu, hit->dpdv);
}

int
	plane_intersect(t_ray ray, t_plane plane, float min, t_hit *hit)
{
	float	divisor;
	float	dividend;
	float	t;

	divisor = vec_dot(ray.dir, plane.normal);
	if (divisor == 0.0)
		return (0);
	dividend = vec_dot(vec_sub(plane.pos, ray.org), plane.normal);
	t = dividend / divisor;
	hit->t = t;
	hit->pos = ray_at(ray, t);
	hit->geometric_normal = plane.normal;
	hit->shading_normal = plane.normal;
	return (t >= min);
}
