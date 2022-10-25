/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   node2.c                                        #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:26 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:26 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "accel.h"

#include "world.h"

float
	split_pos(t_accel_node node)
{
	return (node.u_a.split);
}

t_uint32
	nprims(t_accel_node node)
{
	return (node.u_b.nprims >> 2);
}

t_uint32
	split_axis(t_accel_node node)
{
	return (node.u_b.flags & 3);
}

t_uint32
	is_leaf(t_accel_node node)
{
	return (split_axis(node) == 3);
}

t_uint32
	above_child(t_accel_node node)
{
	return (node.u_b.above_child >> 2);
}
