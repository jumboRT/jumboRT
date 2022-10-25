/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   common_tex.c                                   #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:32 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:32 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "world.h"
#include "mat.h"

GLOBAL unsigned char
	*get_tex_data(GLOBAL t_world *world, t_uint32 index)
{
	return (world->texture_data + index);
}

const GLOBAL unsigned char
	*get_tex_data_const(const GLOBAL t_world *world, t_uint32 index)
{
	return (world->texture_data + index);
}

const GLOBAL t_tex
	*get_tex_const(const GLOBAL t_world *world, t_uint32 index)
{
	return (world->textures + index);
}
