/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   basis.c                                        #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:21 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:21 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "rtmath.h"

t_vec2
	vec_change_basis2(t_vec v, t_vec tangent, t_vec bit_tangent)
{
	return (vec2(vec_dot(v, tangent), vec_dot(v, bit_tangent)));
}
