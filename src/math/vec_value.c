/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   vec_value.c                                    #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:25 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:25 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "rtmath.h"

float
	vec_minv(t_vec v)
{
	return (rt_min(rt_min(rt_min(x(v), y(v)), z(v)), w(v)));
}

float
	vec_maxv(t_vec v)
{
	return (rt_max(rt_max(rt_max(x(v), y(v)), z(v)), w(v)));
}

float
	vec_xyz_maxv(t_vec v)
{
	return (rt_max(rt_max(x(v), y(v)), z(v)));
}
