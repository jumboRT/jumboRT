/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   init.c                                         #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:16 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:16 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "tex.h"

void
	texture_init(t_tex *tex)
{
	tex->type = RT_TEX_COLOR;
	tex->a.color = vec(0, 0, 0, 0);
}
