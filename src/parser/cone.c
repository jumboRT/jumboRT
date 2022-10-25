/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   cone.c                                         #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:07 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:07 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

#include "shape.h"
#include "world_impl.h"

void
	rt_exec_cone(t_world *world, t_parse_ctx *ctx)
{
	t_shape_cone	shape;

	shape.base.data = RT_SHAPE_CONE;
	shape.cone.pos = rt_vec(ctx);
	shape.cone.dir = rt_vec_norm(ctx);
	shape.cone.angle = rt_float(ctx);
	shape.cone.height = rt_float(ctx);
	rt_material(ctx, world, &shape.base);
	world_add_primitive(world, &shape, sizeof(shape));
}
