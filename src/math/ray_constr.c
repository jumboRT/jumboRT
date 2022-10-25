/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   ray_constr.c                                   #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:23 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:23 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "rtmath.h"

t_ray
	ray(t_vec org, t_vec dir)
{
	t_ray	result;

	result.org = org;
	result.dir = dir;
	return (result);
}

t_vec
	ray_at(t_ray ray, float t)
{
	return (vec_add(ray.org, vec_scale(ray.dir, t)));
}
