/* ************************************************************************** */
/*                                                                            */
/*                                                                            */
/*   sqrt.c                                         #  # #  #   #  ##   ###   */
/*                                                  #  # #  ## ##  # #  # #   */
/*   By: csteenvo <csteenvo@student.codam.nl>     # #  # #  # # #  ##   # #   */
/*                                                # #  # #  #   #  # #  # #   */
/*   Created: 2022/10/25 12:02:24 by csteenvo     ###  ###  #   #  ##   ###   */
/*   Updated: 2022/10/25 12:02:24 by csteenvo                                 */
/*                                                                            */
/* ************************************************************************** */

#include "rtmath.h"

#ifndef RT_OPENCL

float
	rt_sqrt(float a)
{
	return (sqrtf(a));
}

float
	rt_cbrt(float a)
{
	return (cbrtf(a));
}

#else

float
	rt_sqrt(float a)
{
	return (sqrt(a));
}

float
	rt_cbrt(float a)
{
	return (cbrt(a));
}

#endif
