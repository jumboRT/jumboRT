/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   polynomial.c                                   #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:25 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:25 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "rtmath.h"

int
	quadratic_solve(const t_quadratic *quadratic, float solutions[2])
{
	float	q;
	float	discriminant;

	discriminant = (quadratic->b * quadratic->b)
		- (4.0 * quadratic->a * quadratic->c);
	if (discriminant < 0.0)
		return (0);
	discriminant = rt_sqrt(discriminant);
	if (quadratic->b < 0)
		q = -0.5 * (quadratic->b - discriminant);
	else
		q = -0.5 * (quadratic->b + discriminant);
	solutions[0] = q / quadratic->a;
	solutions[1] = quadratic->c / q;
	if (solutions[0] > solutions[1])
	{
		q = solutions[0];
		solutions[0] = solutions[1];
		solutions[1] = q;
	}
	return (1);
}
