/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   vec2_arith_fast.c                              #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:21 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:21 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "rtmath.h"

#if defined RT_VECTORIZE

__attribute__ ((const))
t_vec2
	vec2_neg(t_vec2 v)
{
	return (-v);
}

__attribute__ ((const))
t_vec2
	vec2_scale(t_vec2 v, float s)
{
	t_vec2	b;

	b = vec2(s, s);
	return (v * b);
}

__attribute__ ((const))
t_vec2
	vec2_add(t_vec2 a, t_vec2 b)
{
	return (a + b);
}

__attribute__ ((const))
t_vec2
	vec2_sub(t_vec2 a, t_vec2 b)
{
	return (a - b);
}

__attribute__ ((const))
t_vec2
	vec2_mul(t_vec2 a, t_vec2 b)
{
	return (a * b);
}

#endif
