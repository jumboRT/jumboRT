/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   vec_constr_fast2.c                             #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:21 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:21 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "rtmath.h"

#if defined RT_VECTORIZE

t_vec
	vec3(float x, float y, float z)
{
	return (vec(x, y, z, 0.0));
}

t_vec
	vec_0(void)
{
	return (vec(0.0, 0.0, 0.0, 0.0));
}

#endif
