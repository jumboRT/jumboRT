/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   cone_sample.c                                  #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:24 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:24 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "rtmath.h"
#include "sample.h"

static float
	cone_cap_area(t_cone cone)
{
	float	r;

	r = rt_tan(cone.angle) * cone.height;
	return (RT_PI * r * r);
}

static float
	cone_mantle_area(t_cone cone)
{
	float	r;
	float	l;

	r = rt_tan(cone.angle) * cone.height;
	l = rt_sqrt(r * r + cone.height * cone.height);
	return (RT_PI * r * l);
}

t_vec
	cone_sample(t_cone cone, GLOBAL t_context *ctx)
{
	float	r;
	t_vec	douv[4];
	float	sample;

	r = rt_tan(cone.angle) * cone.height;
	douv[2] = vec_tangent(cone.dir);
	douv[3] = vec_cross(cone.dir, douv[2]);
	sample = rt_random_float_range(&ctx->seed, 0, cone_area(cone));
	if (sample < cone_mantle_area(cone))
	{
		douv[0] = vec_scale(cone.dir,
				rt_sqrt(rt_random_float_range(&ctx->seed, 0, cone.height)));
		douv[1] = vec_scale(vec_rotate(cone.dir, douv[2],
					rt_random_float_range(&ctx->seed, 0, RT_2PI)), r);
		return (vec_add(vec_add(douv[0], douv[1]), cone.pos));
	}
	else
	{
		return (vec_add(vec_add(vec_scale(cone.dir, cone.height),
					rt_random_in_disk(&ctx->seed, douv[2], douv[3], r)),
				cone.pos));
	}
}

float
	cone_area(t_cone cone)
{
	return (cone_cap_area(cone) + cone_mantle_area(cone));
}
