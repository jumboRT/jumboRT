/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   node1.c                                        #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:26 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:26 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "accel.h"

#include "world.h"

const GLOBAL t_uint32
	*node_prims(const GLOBAL t_world *world, const GLOBAL t_accel_node *node)
{
	if (nprims(*node) <= ACCEL_INLINE_PRIMS)
		return (node->u_a.inline_primitives);
	else
		return (world->accel_indices + node->u_a.primitive_offset);
}
