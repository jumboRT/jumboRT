/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   max.c                                          #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:20 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:20 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "rtmath.h"

#ifndef RT_OPENCL

float
	rt_max(float a, float b)
{
	return (fmax(a, b));
}

#else

float
	rt_max(float a, float b)
{
	return (fmax(a, b));
}

#endif
