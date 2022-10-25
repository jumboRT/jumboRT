/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   vec2_get.c                                     #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:23 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:23 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "rtmath.h"

#if !defined RT_VECTORIZE

float
	u(t_vec2 v)
{
	return (v.x);
}

float
	v(t_vec2 v)
{
	return (v.y);
}

#endif
