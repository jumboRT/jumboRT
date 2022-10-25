/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   abs.c                                          #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:23 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:23 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "rtmath.h"

#ifndef RT_OPENCL

float
	rt_abs(float a)
{
	return (fabs(a));
}

#else

float
	rt_abs(float a)
{
	return (fabs(a));
}

#endif
