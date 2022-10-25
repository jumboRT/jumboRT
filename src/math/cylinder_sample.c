/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   cylinder_sample.c                              #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:20 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:20 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "rtmath.h"
#include "sample.h"

static float
	cylinder_cap_area(t_cylinder cylinder)
{
	return (RT_PI * cylinder.radius * cylinder.radius);
}

static float
	cylinder_mantle_area(t_cylinder cylinder)
{
	return (RT_2PI * cylinder.radius * cylinder.height);
}

t_vec
	cylinder_sample(t_cylinder cyl, GLOBAL t_context *ctx)
{
	float	sample;
	t_vec	up;
	t_vec	out;
	t_vec	u;
	t_vec	v;

	sample = rt_random_float_range(&ctx->seed, 0, cylinder_area(cyl));
	u = vec_tangent(cyl.dir);
	v = vec_cross(cyl.dir, u);
	if (sample < cylinder_mantle_area(cyl))
	{
		up = vec_scale(cyl.dir, rt_random_float_range(&ctx->seed, 0,
					cyl.height));
		out = vec_scale(vec_rotate(cyl.dir, u,
					rt_random_float_range(&ctx->seed, 0, RT_2PI)), cyl.radius);
		return (vec_add(vec_add(up, out), cyl.pos));
	}
	else if (sample < cylinder_mantle_area(cyl)
		+ cylinder_cap_area(cyl))
		return (vec_add(rt_random_in_disk(&ctx->seed, u, v, cyl.radius),
				cyl.pos));
	else
		return (vec_add(vec_add(vec_scale(cyl.dir, cyl.height),
					rt_random_in_disk(&ctx->seed, u, v, cyl.radius)), cyl.pos));
}

float
	cylinder_area(t_cylinder cylinder)
{
	return (2.0f * cylinder_cap_area(cylinder)
		+ cylinder_mantle_area(cylinder));
}
