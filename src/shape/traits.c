/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   traits.c                                       #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:20 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:20 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "shape.h"

t_uint32
	prim_type(const GLOBAL t_primitive *prim)
{
	return (prim->data & 0xFF);
}

int
	prim_is_infinite(const GLOBAL t_primitive *prim)
{
	if (prim_type(prim) == RT_SHAPE_PLANE)
		return (1);
	if (prim_type(prim) == RT_SHAPE_PARABOLOID)
		return (1);
	if (prim_type(prim) == RT_SHAPE_HYPERBOLOID)
		return (1);
	return (0);
}

int
	prim_is_degenerate(const GLOBAL t_primitive *prim)
{
	if (prim_is_infinite(prim))
		return (1);
	if (prim_type(prim) == RT_SHAPE_POINT)
		return (1);
	return (0);
}
