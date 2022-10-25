/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   paraboloid.c                                   #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:25 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:25 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "rtmath.h"

t_paraboloid
	paraboloid(t_vec pos, float a, float b)
{
	t_paraboloid	result;

	result.pos = pos;
	result.a = a;
	result.b = b;
	return (result);
}

void
	para_hit_info(t_ray ray, t_paraboloid paraboloid, t_hit *hit)
{
	t_vec	oc;

	(void) ray;
	oc = vec_sub(hit->pos, paraboloid.pos);
	hit->geometric_normal
		= vec_norm(vec3(
				2.0f * rt_pow(paraboloid.a, -2.0f) * x(oc),
				2.0f * rt_pow(paraboloid.b, -2.0f) * y(oc),
				-1.0f));
	hit->shading_normal = hit->geometric_normal;
	hit->dpdu = vec_tangent(hit->geometric_normal);
	hit->dpdv = vec_norm(vec_cross(hit->dpdu, hit->geometric_normal));
	hit->uv = vec2(x(hit->pos), y(hit->pos));
}

int
	para_intersect(t_ray ray, t_paraboloid paraboloid, float min, t_hit *hit)
{
	t_quadratic	quadratic;
	t_vec		oc;
	t_vec		dir;
	float		t[2];

	oc = vec_sub(ray.org, paraboloid.pos);
	oc = vec(x(oc) / paraboloid.a, y(oc) / paraboloid.b, z(oc), 0);
	dir = vec(x(ray.dir) / paraboloid.a, y(ray.dir) / paraboloid.b,
			z(ray.dir), 0);
	quadratic.a = x(dir) * x(dir) + y(dir) * y(dir);
	quadratic.b = 2 * (x(oc) * x(dir) + y(oc) * y(dir)) - z(dir);
	quadratic.c = x(oc) * x(oc) + y(oc) * y(oc) - z(oc);
	if (quadratic_solve(&quadratic, t) == 0)
		return (0);
	if (t[0] < min)
		t[0] = t[1];
	if (t[1] < t[0] && t[1] >= min)
		t[0] = t[1];
	else if (t[0] < min)
		return (0);
	hit->t = t[0];
	hit->pos = ray_at(ray, t[0]);
	return (1);
}
