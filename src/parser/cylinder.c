/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   cylinder.c                                     #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:06 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/27 13:02:23 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

#include "shape.h"
#include "world_impl.h"

void
	rt_exec_cylinder(t_world *world, t_parse_ctx *ctx)
{
	t_shape_cylinder	shape;

	shape.base.data = RT_SHAPE_CYLINDER;
	shape.cylinder.pos = rt_vec(ctx);
	shape.cylinder.dir = rt_vec_norm(ctx);
	shape.cylinder.radius = rt_float(ctx) / 2;
	shape.cylinder.height = rt_float(ctx);
	rt_material(ctx, world, &shape.base);
	world_add_primitive(world, &shape, sizeof(shape));
}
