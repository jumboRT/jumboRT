/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   common.c                                       #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:28 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 13:37:28 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "world.h"
#include "mat.h"

t_vec
	get_vertex(const GLOBAL t_world *world, t_uint32 index)
{
	return (world->vertices[index].pos);
}

const GLOBAL t_bxdf
	*get_bxdf_const(const GLOBAL t_world *world, t_uint32 index)
{
	return (&world->bxdfs[index].base);
}

GLOBAL t_bxdf
	*get_bxdf(GLOBAL t_world *world, t_uint32 index)
{
	return (&world->bxdfs[index].base);
}
