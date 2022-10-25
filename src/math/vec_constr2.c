/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   vec_constr2.c                                  #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:21 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:21 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "rtmath.h"

#if !defined RT_VECTORIZE

t_vec
	vec3(float x, float y, float z)
{
	t_vec	result;

	result.x = x;
	result.y = y;
	result.z = z;
	result.w = 0;
	return (result);
}
#endif
