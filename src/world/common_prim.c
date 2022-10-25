/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   common_prim.c                                  #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:28 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:28 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "world.h"
#include "shape.h"
#include "mat.h"

t_uint32
	prim_mat(const GLOBAL t_primitive *prim)
{
	return (prim->data >> 8);
}

const GLOBAL t_primitive
	*get_prim_const(const GLOBAL t_world *world, t_uint32 index)
{
	return ((const GLOBAL t_primitive *)((const GLOBAL char *)world->primitives
		+ (t_uint64)index * RT_PRIMITIVE_ALIGN));
}

GLOBAL t_primitive
	*get_prim(GLOBAL t_world *world, t_uint32 index)
{
	return ((GLOBAL t_primitive *)((GLOBAL char *)world->primitives
		+ (t_uint64)index * RT_PRIMITIVE_ALIGN));
}

const GLOBAL t_material
	*get_mat_const(const GLOBAL t_world *world, t_uint32 index)
{
	return (&world->materials[index]);
}

GLOBAL t_material
	*get_mat(GLOBAL t_world *world, t_uint32 index)
{
	return (&world->materials[index]);
}
