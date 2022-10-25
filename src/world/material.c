/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   material.c                                     #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:28 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:28 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "world.h"
#include "mat.h"

#include <limits.h>

void
	material_init(t_material *mat, const t_world *world)
{
	mat->flags = 0;
	mat->surface.begin = world->bxdfs_count;
	mat->surface.end = world->bxdfs_count;
	mat->surface.weight = 0.0;
	mat->volume.begin = world->bxdfs_count;
	mat->volume.end = world->bxdfs_count;
	mat->volume.weight = 0.0;
	mat->brightness = 0.0;
	mat->emission_exp = 0.0;
	mat->refractive_index = 1.0;
}
