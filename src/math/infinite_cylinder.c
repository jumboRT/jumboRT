/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   infinite_cylinder.c                            #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:25 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:25 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "rtmath.h"

int
	infinite_cylinder_intersect(t_ray relative_ray, float radius,
			float intersections[2])
{
	t_quadratic	quadratic;

	quadratic.a = rt_pow(x(relative_ray.dir), 2.0)
		+ rt_pow(y(relative_ray.dir), 2.0);
	quadratic.b = 2.0 * (x(relative_ray.dir) * x(relative_ray.org)
			+ y(relative_ray.dir) * y(relative_ray.org));
	quadratic.c = rt_pow(x(relative_ray.org), 2.0)
		+ rt_pow(y(relative_ray.org), 2.0) - (radius * radius);
	if (quadratic_solve(&quadratic, intersections) == 0)
		return (0);
	return (1);
}
