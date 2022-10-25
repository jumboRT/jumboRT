/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   cone_util.c                                    #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:23 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:23 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "rtmath.h"

int
	hit_on_finite_cone(t_vec hit, t_cone cone)
{
	float	dot;

	dot = vec_dot(vec_sub(hit, cone.pos), cone.dir);
	return (dot >= 0 && dot <= cone.height);
}

t_vec
	cone_normal_at(t_vec hit, t_cone cone)
{
	t_vec	relative_hit;

	relative_hit = vec_sub(hit, cone.pos);
	return (vec_norm(vec_cross(relative_hit,
				vec_cross(relative_hit, cone.dir))));
}

t_vec2
	cone_uv_mantle(t_cone cone, float radius, t_vec point)
{
	t_vec	op;

	op = vec_sub(point, cone.pos);
	return (vec2_add(
			vec2_scale(
				vec_change_basis2(op,
					vec_x(1.0),
					vec_y(1.0)), 1.0 / (4.0 * radius)), vec2(0.25, 0.25)));
}

t_vec2
	cone_uv_cap(t_cone cone, float radius, t_vec point)
{
	t_vec	op;
	t_vec	tangent;
	t_vec2	uv;

	op = vec_sub(point, cone.pos);
	tangent = vec_norm(vec_tangent(cone.dir));
	uv = vec_change_basis2(op, tangent, vec_norm(vec_cross(tangent, cone.dir)));
	uv = vec2_scale(uv, 1.0 / (4.0 * radius));
	uv = vec2_add(uv, vec2(0.75, 0.25));
	return (uv);
}
