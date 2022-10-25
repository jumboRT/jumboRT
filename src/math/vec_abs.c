/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   vec_abs.c                                      #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:25 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:25 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "rtmath.h"

#if !defined RT_VECTORIZE

t_vec
	vec_abs(t_vec v)
{
	return (vec(rt_abs(x(v)), rt_abs(y(v)), rt_abs(z(v)), rt_abs(w(v))));
}

#endif
