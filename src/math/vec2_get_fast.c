/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   vec2_get_fast.c                                #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:24 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:24 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "rtmath.h"

#if defined RT_VECTORIZE

float
	u(t_vec2 v)
{
	t_vec2_conv	conv;

	conv.vec = v;
	return (conv.elem[0]);
}

float
	v(t_vec2 v)
{
	t_vec2_conv	conv;

	conv.vec = v;
	return (conv.elem[1]);
}

#endif
