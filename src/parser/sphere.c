/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   sphere.c                                       #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:05 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:05 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

#include "shape.h"
#include "world_impl.h"

void
	rt_exec_sphere(t_world *world, t_parse_ctx *ctx)
{
	t_shape_sphere	sphere;

	sphere.base.data = RT_SHAPE_SPHERE;
	sphere.pos = rt_vec(ctx);
	sphere.radius = rt_float(ctx) / 2;
	rt_material(ctx, world, &sphere.base);
	world_add_primitive(world, &sphere, sizeof(sphere));
}
